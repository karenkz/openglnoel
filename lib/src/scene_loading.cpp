void loadglTFObjScene(const fs::path & objPath, const fs::path & mtlBaseDir, SceneData & data, bool loadTextures){

    tinygltf::TinyGLTF Loader_;
    tinygltf::Model Model_;
    std::string err,warn;
    bool ret = false;

    //const std::string filename = "/home/karen/Documents/Synthese_image_avancé/openglnoel-cheat-gold2/models/box01.glb" ;
    const std::string filename = "/home/karen/Documents/Synthese_image_avancé/openglnoel/models/test/DamagedHelmet.glb";
    const std::string ext = GetFilePathExtension(filename);

    if (ext == "glb") {
    // binary glTF.
    std::cout<<"Fichier de format glb"<<std::endl;
    ret = Loader_.LoadBinaryFromFile(&Model_,&err,&warn,filename);
    } else {
    //ASCII glTF.
    ret = Loader_.LoadASCIIFromFile(&Model_, &err, &warn, filename);
    }
 
    if (!err.empty()) { 
        std::cerr << err << std::endl;
    }

    if (!ret) {
        throw std::runtime_error(err);
    }

     //Affichage des données
     std::cout << "Fichier glTF:\n"
            << Model_.accessors.size() << " accessors\n"
            << Model_.animations.size() << " animations\n"
            << Model_.buffers.size() << " buffers\n"
            << Model_.bufferViews.size() << " bufferViews\n"
            << Model_.materials.size() << " materials\n"
            << Model_.meshes.size() << " meshes\n"
            << Model_.nodes.size() << " nodes\n"
            << Model_.textures.size() << " textures\n"
            << Model_.images.size() << " images\n"
            << Model_.skins.size() << " skins\n"
            << Model_.samplers.size() << " samplers\n"
            << Model_.cameras.size() << " cameras\n"
            << Model_.scenes.size() << " scenes\n"
            << Model_.lights.size() << " lights\n";

    for (const auto &meshs : Model_.meshes) {
	std::cout << "Mesh courant " << meshs.primitives.size() << " primitives:\n";

    	// Création mesh courant
    	tinygltf::Mesh loadedMesh;
    	
	// To store the min and max of the buffer (as 3D vector of floats)
    	v3f pMin = {}, pMax = {};

    	// Store the name of the glTF mesh (if defined)
    	loadedMesh.name = meshs.name;

	for (const auto &primitive_ : meshs.primitives) {
	    
		//
	    std::cout << "primitive courant " << primitive_.attributes.size() << " attributs,\n"
	    		<< primitive_.indices << " indice,\n"
	    		<< primitive_.mode << " mode,\n"
	    		<< primitive_.material << " index materiel,\n"
		   	<< primitive_.targets.size() <<" taille vecteur de map\n";

	    for(const auto &t : primitive_.targets){
		std::cout<<"taille map"<<t.size()<<"\n";
		for (std::map<std::string,int>::const_iterator it=t.begin(); it!=t.end(); ++it)
    			std::cout << it->first << " => " << it->second << '\n';

	    }
	    std::cout<<"target:\n";


		//
	    
	    const auto &indicesAccessor = Model_.accessors[primitive_.indices];
		std::cout<<"type accessor:"<<indicesAccessor.type<<std::endl;
		std::cout<<"componentType accessor:"<<indicesAccessor.componentType<<std::endl;

            const auto &bufferView = Model_.bufferViews[indicesAccessor.bufferView];//acces au buffer, un vao par primitive
            const auto &buffer = Model_.buffers[bufferView.buffer];
            const auto dataAddress = buffer.data.data() + bufferView.byteOffset +
                                 indicesAccessor.byteOffset;
            const auto byteStride = indicesAccessor.ByteStride(bufferView);
            const auto count = indicesAccessor.count;


//charger les buffer

	    switch (meshPrimitive.mode) {
		case TINYGLTF_MODE_TRIANGLES:  // this is the simplest case to handle

        	{
          		std::cout << "TRIANGLES\n";
	    		for (auto &attrib : primitive_.attributes) {
			 const auto attribAccessor = Model_.accessors[attribute.second];
            		const auto &bufferView = Model_.bufferViews[attribAccessor.bufferView];
            		const auto &buffer = Model_.buffers[bufferView.buffer];
            		const auto dataPtr = buffer.data.data() + bufferView.byteOffset +
                                 attribAccessor.byteOffset;
            		const auto byte_stride = attribAccessor.ByteStride(bufferView);
            		const auto count = attribAccessor.count;

           		 std::cout << "current attribute has count " << count
                      << " and stride " << byte_stride << " bytes\n";

            		std::cout << "attribut : " << attrib.first << '\n';
      			int vaa = -1;
            		if (attrib.first == "POSITION") {
             			 std::cout << "found position attribute\n";
      				tinygltf::Accessor accessor = Model_.accessors[attrib.second];
      				int byteStride = accessor.ByteStride(Model_.bufferViews[accessor.bufferView]);
      				//glBindBuffer(GL_ARRAY_BUFFER, vbos[accessor.bufferView]);
				
 				pMin.x = (float)attrib.minValues[0];
              			pMin.y = (float)attrib.minValues[1];
              			pMin.z = (float)attrib.minValues[2];
              			pMax.x = (float)attrib.maxValues[0];
             			pMax.y = (float)attrib.maxValues[1];
              			pMax.z = (float)attrib.maxValues[2];
      				int size = 1;
      				if (accessor.type != TINYGLTF_TYPE_SCALAR) {
        				size = accessor.type;
      				}

				vaa=0;
				std::cout<<attrib.second<<std::endl;

			}
      			if (attrib.first.compare("NORMAL") == 0) { vaa = 1; std::cout<<attrib.second<<std::endl;}
      			if (attrib.first.compare("TEXCOORD_0") == 0) { vaa = 2; std::cout<<attrib.second<<std::endl;}
      			if (vaa > -1) {
       		   	 	glEnableVertexAttribArray(vaa);
         			glVertexAttribPointer(vaa, size, accessor.componentType,accessor.normalized ? GL_TRUE : GL_FALSE,
                              byteStride, BUFFER_OFFSET(accessor.byteOffset));*/std::cout<<"ojfzof\n";
      				}else{
        	    			std::cout << "vaa missing: " << attrib.first << std::endl;
				}

6    	   		 }//fin attrib
		}
	    }
	    

	}//fin parcours primitive de Mesh
    }//fin parcours mesh de model

	


}//fin load tiny_gltf


}//fin namespace
