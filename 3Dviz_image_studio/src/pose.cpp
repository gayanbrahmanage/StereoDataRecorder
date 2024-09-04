#include<pose.h>
pose::pose(){

}

pose::~pose(){
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

void pose::init_pose(float _scale, std::string type){

  scale=_scale;
  draw_type=type;
  if(draw_type=="dot"){
    vertices_size=6; // number of elements
    indices_size=1;
  }
  if(draw_type=="cam"){
    vertices_size=8*6;
    indices_size=26;
  }

  float vertices[vertices_size]={0};
  unsigned int indices[indices_size]={0};

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)/* n points */, vertices, GL_STATIC_DRAW);

  // 3. copy our index array in a element buffer for OpenGL to use
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)/* 12 per pose lines */, indices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6/* number of atrributes per point*/ * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0);
}


void pose::draw_pose(std::vector<Eigen::Matrix4d,Eigen::aligned_allocator<Eigen::Matrix4d>> pose){

  float vertices[vertices_size]={0.0f};
  unsigned int indices[indices_size]={0};

  if(draw_type=="dot"){
    for (int i=0; i<pose.size(); i++){

      Eigen::Matrix4d p=pose[i];

      vertices[0]=p(0,3);
      vertices[1]=p(1,3);
      vertices[2]=p(2,3);

      vertices[3]=0.0f;
      vertices[4]=1.0f;
      vertices[5]=0.0f;

      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) /* n points */, vertices, GL_STATIC_DRAW);

      glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
      glDrawArrays(GL_POINTS, 0, pose.size());
    }

  } else if(draw_type=="cam"){
    float size=1.0f*scale;

    std::vector<Eigen::Vector4d,Eigen::aligned_allocator<Eigen::Vector4d>> points;
    unsigned int indices_per_camera[26]={0,1, 1,2, 2,3, 3,0, 4,5, 5,6, 6,7, 7,4, 0xFFFF, 2,6, 0xFFFF, 3,7,0xFFFF, 1,5, 0xFFFF};

    Eigen::Vector4d p1(size, -size, size, 1.0f);
    points.push_back(p1);
    Eigen::Vector4d p2(-size, -size, size, 1.0f);
    points.push_back(p2);
    Eigen::Vector4d p3(-size, size, size, 1.0f);
    points.push_back(p3);
    Eigen::Vector4d p4(size, size, size, 1.0f);
    points.push_back(p4);

    size=3.0f*scale;
    Eigen::Vector4d p11(size, -size, -size, 1.0f);
    points.push_back(p11);
    Eigen::Vector4d p12(-size, -size, -size, 1.0f);
    points.push_back(p12);
    Eigen::Vector4d p13(-size, size, -size, 1.0f);
    points.push_back(p13);
    Eigen::Vector4d p14(size, size, -size, 1.0f);
    points.push_back(p14);
    for (int i=0; i<pose.size(); i++){
      Eigen::Matrix4d p=pose[i];
      for (int j=0; j<8; j++){

        Eigen::Vector4d pt=p*points[j];

        vertices[6*j+0]=pt(0);
        vertices[6*j+1]=pt(1);
        vertices[6*j+2]=pt(2);

        vertices[6*j+3]=0.0f;
        vertices[6*j+4]=1.0f;
        vertices[6*j+5]=0.0f;

        //indices[8*i+j]=8*i+j;
      }

      for(int j=0; j<26; j++){
        indices[j]=indices_per_camera[j];
      }
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) /* n points */, vertices, GL_STATIC_DRAW);
      // 3. copy our index array in a element buffer for OpenGL to use
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)/* 12 per pose lines */, indices, GL_STATIC_DRAW);

      glBindVertexArray(VAO);
      glEnable(GL_PRIMITIVE_RESTART);
      glPrimitiveRestartIndex(0xFFFF);
      glDrawElements(GL_LINE_STRIP, 26, GL_UNSIGNED_INT, 0);

      glBindVertexArray(0);
    }

  } else{

  }
}
