//
//  LoadScene.cpp
//  DaliIsland
//
//  Created by Chiara Lurgo on 15/06/16.
//  Copyright © 2016 Luca Mezzatesta. All rights reserved.
//

#include "LoadScene.hpp"

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)
#define TRUE                1
#define FALSE               0

const struct aiScene* scene = NULL;
GLuint scene_list_opera1 = 0;
GLuint scene_list_opera2 = 0;
GLuint scene_list_opera3 = 0;
GLuint scene_list_opera4 = 0;
GLuint scene_list_terreno = 0;
GLuint scene_list_nuvole = 0;

//coordinate scena
aiVector3D scene_min, scene_max, scene_center;
//static float angle = 0.f;

//elementi blend (da settare!!!)
static const std::string basepath = "./";

//luci
GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]= { 0.0f, 0.0f, 15.0f, 1.0f };

//texture
std::map<std::string, GLuint*> textureIdMap;	// map image filenames to textureIds
GLuint*		textureIds;							// pointer to texture Array


/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/

void color4_to_float4(const aiColor4D *c, float f[4])
{
    f[0] = c->r;
    f[1] = c->g;
    f[2] = c->b;
    f[3] = c->a;
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/

void set_float4(float f[4], float a, float b, float c, float d)
{
    f[0] = a;
    f[1] = b;
    f[2] = c;
    f[3] = d;
}

/*---------------------------------------------------------------------------------------------------------------------------------------*/

void apply_material(const struct aiMaterial *mtl)
{
    float c[4];
    
    GLenum fill_mode;
    int ret1, ret2;
    aiColor4D diffuse;
    aiColor4D specular;
    aiColor4D ambient;
    aiColor4D emission;
    float shininess, strength;
    int two_sided;
    int wireframe;
    int max;
    
    int texIndex = 0;
    aiString texPath;	//contains filename of texture
    if(AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath))
    {
        //bind texture
        unsigned int texId = *textureIdMap[texPath.data];
        glBindTexture(GL_TEXTURE_2D, texId);
    }
    
    set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
    if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
        color4_to_float4(&diffuse, c);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);
    
    set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
    if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
        color4_to_float4(&specular, c);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
    
    set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
    if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
        color4_to_float4(&ambient, c);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);
    
    set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
    if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
        color4_to_float4(&emission, c);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);
    
    max = 1;
    ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, (unsigned int *)&max);
    max = 1;
    ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, (unsigned int *)&max);
    if((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
    else {
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
        set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
    }
    
    max = 1;
    if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, (unsigned int *)&max))
        fill_mode = wireframe ? GL_LINE : GL_FILL;
    else
        fill_mode = GL_FILL;
    glPolygonMode(GL_FRONT_AND_BACK, fill_mode);
    
    max = 1;
    if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, (unsigned int *)&max)) && two_sided)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/

void Color4f(const aiColor4D *color){
    glColor4f(color->r, color->g, color->b, color->a);
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/

void recursive_render(const struct aiScene *sc, const struct aiNode* nd, float scale){
    unsigned int i, t, n = 0;
    aiMatrix4x4 m = nd->mTransformation;
    
    printf("Node name: %s\n",nd->mName.data);
    
    m.Scaling(aiVector3D(scale, scale, scale), m);
    
    // update transform
    m.Transpose();
    glPushMatrix();
    glMultMatrixf((float*)&m);
    
    //disegno tutte le mesh sotto questo nodo
    for (; n < nd->mNumMeshes; ++n){
        //prendo una mesh per volta legata a questo nodo
        const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
        printf("Drawing MESH with this name: %s\n",mesh->mName.data);
        
        apply_material(sc->mMaterials[mesh->mMaterialIndex]);
        
        //se la mesh ha una texture abilito la texture
        if(mesh->HasTextureCoords(0))
            glEnable(GL_TEXTURE_2D);
        else
            glDisable(GL_TEXTURE_2D);
        
        //se la mesh ha le normali abilito la luce
        if(mesh->mNormals == NULL)
            glDisable(GL_LIGHTING);
        else
            glEnable(GL_LIGHTING);
        
        //se la mesh ha un colore abilito il colore
        if(mesh->mColors[0] != NULL)
            glEnable(GL_COLOR_MATERIAL);
        else
            glDisable(GL_COLOR_MATERIAL);
        
        
        //disegno tutte le facce della mesh
        for (t = 0; t < mesh->mNumFaces; ++t) {
            const struct aiFace* face = &mesh->mFaces[t];
            GLenum face_mode;
            
            //in base al numero di indici della faccia capisco come disegnarla
            switch(face->mNumIndices)
            {
                case 1: face_mode = GL_POINTS; break;
                case 2: face_mode = GL_LINES; break;
                case 3: face_mode = GL_TRIANGLES; break;
                default: face_mode = GL_POLYGON; break;
            }
            glBegin(face_mode);
            
            //considero vertice per vertice della mesh
            for(i = 0; i < face->mNumIndices; i++){
                int vertexIndex = face->mIndices[i];
                
                //vado a riprendermi il colore dell'indice  (vedi struttura mColors!)
                if(mesh->mColors[0] != NULL)
                    Color4f(&mesh->mColors[0][vertexIndex]);
                //se c'è applico la texture
                if(mesh->mNormals != NULL)
                    
                    if(mesh->HasTextureCoords(0))		//HasTextureCoords(texture_coordinates_set)
                    {
                        glTexCoord2f(mesh->mTextureCoords[0][vertexIndex].x, 1 - mesh->mTextureCoords[0][vertexIndex].y); //mTextureCoords[channel][vertex]
                    }
                //setto il vettore normale
                glNormal3fv(&mesh->mNormals[vertexIndex].x);
                //setto il vertice
                glVertex3fv(&mesh->mVertices[vertexIndex].x);
            }
            
            glEnd();
            
        }
    }
    
    // draw all children
    for (n = 0; n < nd->mNumChildren; ++n)
    {
        recursive_render(sc, nd->mChildren[n], scale);
    }
    
    glPopMatrix();
}

/*---------------------------------------------------------------------------------------------------------------------------------------*/

int LoadGLTextures(const aiScene* scene)
{
    ILboolean success;
    
    /* Before calling ilInit() version should be checked. */
    if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
    {
        ILint test = ilGetInteger(IL_VERSION_NUM);
        /// wrong DevIL version ///
        std::string err_msg = "Wrong DevIL version. Old devil.dll in system32/SysWow64?";
        char* cErr_msg = (char *) err_msg.c_str();
        
        return -1;
    }
    
    ilInit(); /* Initialization of DevIL */
    
    //if (scene->HasTextures()) abortGLInit("Support for meshes with embedded textures is not implemented");
    
    /* getTexture Filenames and Numb of Textures */
    for (unsigned int m=0; m<scene->mNumMaterials; m++)
    {
        int texIndex = 0;
        aiReturn texFound = AI_SUCCESS;
        
        aiString path;	// filename
        
        while (texFound == AI_SUCCESS)
        {
            texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
            textureIdMap[path.data] = NULL; //fill map with textures, pointers still NULL yet
            texIndex++;
        }
    }
    
    int numTextures = (int) textureIdMap.size();
    
    /* array with DevIL image IDs */
    ILuint* imageIds = NULL;
    imageIds = new ILuint[numTextures];
    
    /* generate DevIL Image IDs */
    ilGenImages(numTextures, imageIds); /* Generation of numTextures image names */
    
    /* create and fill array with GL texture ids */
    textureIds = new GLuint[numTextures];
    glGenTextures(numTextures, textureIds); /* Texture name generation */
    
    /* define texture path */
    //std::string texturepath = "../../../test/models/Obj/";
    
    /* get iterator */
    std::map<std::string, GLuint*>::iterator itr = textureIdMap.begin();
    
    for (int i=0; i<numTextures; i++)
    {
        
        //save IL image ID
        std::string filename = (*itr).first;  // get filename
        (*itr).second =  &textureIds[i];	  // save texture id for filename in map
        itr++;								  // next texture
        
        
        ilBindImage(imageIds[i]); /* Binding of DevIL image name */
        std::string fileloc = basepath + filename;	/* Loading of image */
        success = ilLoadImage((const char *)fileloc.c_str());
        
        fprintf(stdout,"Loading Image: %s\n", fileloc.data());
        
        if (success) /* If no error occured: */
        {
            success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE); /* Convert every colour component into
                                                                 unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
            if (!success)
            {
                /* Error occured */
                fprintf(stderr,"Couldn't convert image");
                return -1;
            }
            //glGenTextures(numTextures, &textureIds[i]); /* Texture name generation */
            glBindTexture(GL_TEXTURE_2D, textureIds[i]); /* Binding of texture name */
            //redefine standard texture values
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear
                                                                               interpolation for magnification filter */
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear
                                                                               interpolation for minifying filter */
            glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
                         ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
                         ilGetData()); /* Texture specification */
        }
        else
        {
            /* Error occured */
            fprintf(stderr,"Couldn't load Image: %s\n", fileloc.data());
        }
    }
    ilDeleteImages(numTextures, imageIds); /* Because we have already copied image data into texture data
                                            we can release memory used by image. */
    
    //Cleanup
    delete [] imageIds;
    imageIds = NULL;
    
    //return success;
    return TRUE;
}

/*---------------------------------------------------------------------------------------------------------------------------------------*/

void get_bounding_box_for_node (const struct aiNode* nd,
                                aiVector3D* min,
                                aiVector3D* max,
                                aiMatrix4x4* trafo
                                ){
    aiMatrix4x4 prev;
    unsigned int n = 0, t;
    
    prev = *trafo;
    aiMultiplyMatrix4(trafo,&nd->mTransformation);
    
    for (; n < nd->mNumMeshes; ++n) {
        const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
        for (t = 0; t < mesh->mNumVertices; ++t) {
            
            aiVector3D tmp = mesh->mVertices[t];
            aiTransformVecByMatrix4(&tmp,trafo);
            
            min->x = aisgl_min(min->x,tmp.x);
            min->y = aisgl_min(min->y,tmp.y);
            min->z = aisgl_min(min->z,tmp.z);
            
            max->x = aisgl_max(max->x,tmp.x);
            max->y = aisgl_max(max->y,tmp.y);
            max->z = aisgl_max(max->z,tmp.z);
        }
    }
    
    for (n = 0; n < nd->mNumChildren; ++n) {
        get_bounding_box_for_node(nd->mChildren[n],min,max,trafo);
    }
    *trafo = prev;
}

/*---------------------------------------------------------------------------------------------------------------------------------------*/

void get_bounding_box (aiVector3D* min, aiVector3D* max)
{
    aiMatrix4x4 trafo;
    aiIdentityMatrix4(&trafo);
    
    min->x = min->y = min->z =  1e10f;
    max->x = max->y = max->z = -1e10f;
    get_bounding_box_for_node(scene->mRootNode,min,max,&trafo);
}

/*---------------------------------------------------------------------------------------------------------------------------------------*/

int importScene(const char* pathName){
    scene = aiImportFile(pathName, aiProcessPreset_TargetRealtime_Quality);
    
    if(!scene){
        return 1;
    }
    else{
        get_bounding_box(&scene_min,&scene_max);
        scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
        scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
        scene_center.z = (scene_min.z + scene_max.z) / 2.0f;
        if (!LoadGLTextures(scene))
            printf("Error");
        return 0;
    }
}


/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/

const struct aiScene* loadScene(const char* pathName){
    importScene(pathName);
    
    return scene;
}

void ReleaseImport(){
    aiReleaseImport(scene);
}