#include "model.h"

Model::Model() {} 
Model::~Model(){
	vertices.clear();
	indices.clear();
}

Model::Model(char* model_filename){
	triangle_mesh = new btTriangleMesh();
	btVector3 triArray[3];

	//set up importer object
	Assimp::Importer importer;
	const aiScene* my_scene = importer.ReadFile(model_filename, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	if(!my_scene){
		throw std::logic_error("Could not open .obj file: " + std::string(model_filename));
	}

	//iterate through the meshes and load vertices and indices for each
	for(unsigned int j = 0; j < my_scene->mNumMeshes; j++){
		aiMesh* mesh = my_scene->mMeshes[j];
		//Process Vertices
		for(unsigned int i = 0; i < mesh->mNumVertices; i++){
			aiVector3D ai_vec = mesh->mVertices[i];
			aiVector3D ai_texture = {0.0,0.5,0.0};
			aiVector3D ai_normal = {0.0,0.0,0.0};
			if(mesh->HasNormals()){
				ai_normal = mesh->mNormals[i];
			}
			glm::vec3 vertex = {ai_vec.x, ai_vec.y, ai_vec.z};
			if(mesh->HasTextureCoords(0)){
				ai_texture = mesh->mTextureCoords[0][i];
			}
			glm::vec2 texture_coordinates = {ai_texture.x,ai_texture.y};
			glm::vec3 normal = {ai_normal.x,ai_normal.y,ai_normal.z};
			vertices.push_back({vertex,normal,texture_coordinates});
		}
		//Process Indices
		for(unsigned int i = 0; i < mesh->mNumFaces; i++){
			aiFace face = mesh->mFaces[i];

			//if we were not given triangles, throw an error
			if(face.mNumIndices != 3){
				std::string error;
				std::string file_name(model_filename);
				error = "Expected triangles in faces from the file: " + file_name + " but received " + std::to_string(face.mNumIndices) + " indices.\n";
				throw std::logic_error(error);
			}
			for(unsigned int k = 0; k < face.mNumIndices; k++ ){
				aiVector3D position = mesh->mVertices[face.mIndices[k]];
				triArray[k] = btVector3(position.x, position.y, position.z);
				indices.push_back(face.mIndices[k]);
			}
			triangle_mesh->addTriangle(triArray[0], triArray[1], triArray[2]);
		}
	}

	//set up vertex and index buffers
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

void Model::Render(){
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texture_coordinates));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

btTriangleMesh* Model::GetTriangleMesh(){
	return triangle_mesh;
}
