#include "viz.h"
#include "shader.h"

bool right_click_pressed=false;
//model
float axis_rotation_angle=0.0f;
float X_axis_rotation_angle=0.0f, Y_axis_rotation_angle=0.0f;
glm::vec3 roation_axis =glm::vec3(0.0f, 1.0f, 0.0f);

// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse_left = true, firstMouse_middle = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  640.0f / 2.0;
float lastY =  480.0 / 2.0;
float fov   =  30.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos){

  if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)){
    if (firstMouse_left){
      lastX = xpos;
      lastY = ypos;
      firstMouse_left = false;
    }

  //if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)){
    //std::cout<<glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)<<std::endl;
    //std::cout<<xpos<<" "<<ypos<<std::endl;
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
  //}

  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  cameraFront = glm::normalize(front);
} else{
  firstMouse_left=true;
}

  if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)){
    if (firstMouse_middle){
      lastX = xpos;
      lastY = ypos;
      firstMouse_middle = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    if(abs(xoffset)>abs(yoffset)){
      axis_rotation_angle+=xoffset;
    } else{
      axis_rotation_angle+=yoffset;
    }

    X_axis_rotation_angle+=xoffset;
    Y_axis_rotation_angle+=yoffset;

    float x_direction=abs(Y_axis_rotation_angle)/(abs(X_axis_rotation_angle)+abs(Y_axis_rotation_angle));
    float y_direction=abs(X_axis_rotation_angle)/(abs(X_axis_rotation_angle)+abs(Y_axis_rotation_angle));
    float z_direction=std::min(abs(X_axis_rotation_angle),abs(Y_axis_rotation_angle))/(abs(X_axis_rotation_angle)+abs(Y_axis_rotation_angle));

    //std::cout<<xoffset << " "<< yoffset<<std::endl;
    roation_axis =glm::vec3(x_direction, y_direction, z_direction);

  }else{
    firstMouse_middle=true;
  }

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
      right_click_pressed=true;
    }else{
      right_click_pressed=false;
    }
        //popup_menu();
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
  fov -= (float)yoffset;
  if (fov < 1.0f)
      fov = 1.0f;
  if (fov > 45.0f)
      fov = 45.0f;
}

void processInput(GLFWwindow *window){
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
  float cameraSpeed = 2.5 * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      cameraPos += cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      cameraPos -= cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
    cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

    X_axis_rotation_angle=0.0f;
    Y_axis_rotation_angle=0.0f;
    axis_rotation_angle=0.0f;
    fov=45.0f;

  }
  if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS){
    cameraPos   = glm::vec3(3.0f, 0.0f, 0.0f);
    cameraFront = glm::vec3(-1.0f, 0.0f, 0.0f);
    cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
  }
  if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS){
    cameraPos   = glm::vec3(0.0f, 0.0f, -3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
    cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
  }
  if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS){
    cameraPos   = glm::vec3(-3.0f, 0.0f, 0.0f);
    cameraFront = glm::vec3(1.0f, 0.0f, 0.0f);
    cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
  }
  if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS){
    cameraPos   = glm::vec3(0.0f, 3.0f, 0.0f);
    cameraFront = glm::vec3(0.0f, -1.0f, 0.0f);
    cameraUp    = glm::vec3(1.0f, 0.0f, 0.0f);
  }
  if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS){
    cameraPos   = glm::vec3(0.0f, -3.0f, 0.0f);
    cameraFront = glm::vec3(0.0f, 1.0f, 0.0f);
    cameraUp    = glm::vec3(1.0f, 0.0f, 0.0f);
  }
}

/*
callback Functions
*/
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// viz class members
viz::viz(){
  //std::cout << "OpenGL Application Object is Created" << std::endl;
}

viz::~viz(){

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  ax.~axes();
  glDeleteProgram(shaderProgram);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();

}

void viz::init(){

  lastX =  (viz_param->width-viz_param-> menu_windwo_width)/ 2.0;
  lastY =  viz_param->height/ 2.0;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  window = glfwCreateWindow(viz_param->width, viz_param->height, "3D Image Studio-Copyright Gayan Brahmanage", NULL, NULL);
  if (window == NULL){
   std::cout << "Failed to create GLFW window" << std::endl;
   glfwTerminate();
  }
  glfwMakeContextCurrent(window);

  //calback functions
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, CursorPosCallback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);

  /*
  GLAD manages function pointers for OpenGL so we want to initialize
  GLAD before we call any OpenGL function:
  */
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    std::cout << "Failed to initialize GLAD" << std::endl;
  }

  // Setup Dear ImGui context
  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //

  // vertex shader
  // build and compile our shader program
  // ------------------------------------
  ourShader=Shader("../3Dviz/shader/shader.vs", "../3Dviz/shader/shader.fs"); // you can name your shader files however you like

}

void viz::run(){
  while (true){
    if(!glfwWindowShouldClose(window)){

        if(right_click_pressed){
          stcam_node->right_click_pressed=true;
          stcam_node_tripple->right_click_pressed=true;
          right_click_pressed=false;
        }
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        /* At the start of frame we want to clear the screen*/
        glClearColor(viz_param->bg_color_R, viz_param->bg_color_G, viz_param->bg_color_B, 0.5f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render here
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // render your IMGUI GUI

        // main menu bar

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Tools")) {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help")) {
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        //////////////////////////////////////////////////////////////////////////
        // menu
        int main_menu_h=20; // clearence from the main menu bar
        ImGui::SetNextWindowPos(ImVec2(0, main_menu_h));
        ImGui::SetNextWindowSize(ImVec2(viz_param->width-viz_param->mavlink_window_width, viz_param->menu1_windwo_height));

        ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize);

        if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None)){
          if (ImGui::BeginTabItem("Config")){

              //left window
              ImGui::SetNextWindowPos(ImVec2(0, 20+viz_param->menu1_windwo_height));
              ImGui::SetNextWindowSize(ImVec2(viz_param->menu2_windwo_width, viz_param->height-viz_param->menu1_windwo_height-20));
              ImGui::Begin("Add Modules", nullptr, ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoResize);

              //ImGui::Text("Select Modules");
              ImGui::Checkbox("Mono Camera Module", &cam_node->add_module);
              if(cam_node->add_module)cam_node->init();

              ImGui::Checkbox("Stereo Camera Module", &stcam_node->add_module);
              if(stcam_node->add_module)stcam_node->init();

              ImGui::Checkbox("Stereo Tripple Camera Module", &stcam_node_tripple->add_module);
              if(stcam_node_tripple->add_module)stcam_node_tripple->init();

              ImGui::Checkbox("Mavlink Module", &mvlink_node->add_module);
              if(mvlink_node->add_module)mvlink_node->init();

              ImGui::End(); // left window

            ImGui::EndTabItem();
          }

          if (ImGui::BeginTabItem("Camera")){
                ImGui::Checkbox("Camera Enable", &cam_node->module_enabled);
                cam_node->run();
                ImGui::Checkbox("Show Camera", &cam_node->camera_show_EN);
                // if stbi image is used
                module_camera(cam_node->ready,cam_node->module_enabled, cam_node->camera_show_EN,
                             cam_node->image_data, cam_node->width, cam_node->height, cam_node->channels);
                //ImGui::Checkbox("Save Images", &cam_node->save_images_EN);
            ImGui::EndTabItem();
          }
          if (ImGui::BeginTabItem("Stereo Camera")){
                ImGui::Checkbox("Stereo Camera Enable", &stcam_node->module_enabled);
                stcam_node->module_enabled=stcam_node->module_enabled&&stcam_node->add_module;
                stcam_node->run();
                ImGui::Checkbox("Show Camera", &stcam_node->camera_show_EN);
                ImGui::Checkbox("Flip Camera", &stcam_node->left_right_flipped);

                //mavlink
                ImGui::SetCursorPos(ImVec2(200.0f, 30.0f));
                ImGui::Checkbox("Show Trim", &stcam_node->show_trimmed);
                ImGui::SetCursorPos(ImVec2(200.0f, 55.0f));
                ImGui::Checkbox("Mavlink", &mvlink_node->mavlink_EN);
                mvlink_node->mavlink_EN=mvlink_node->mavlink_EN&& mvlink_node->add_module;
                mvlink_node->run();
                module_mavlink();



                if(stcam_node->left_right_flipped){
                  module_stereo_camera(stcam_node->ready,stcam_node->module_enabled, stcam_node->camera_show_EN,
                                     stcam_node->image_data2, stcam_node->image_data1,stcam_node->width,
                                     stcam_node->height, stcam_node->channels, stcam_node->top_trim, stcam_node->bot_trim,
                                     stcam_node->cv_image1,stcam_node->cv_image2 );

                }else{
                  module_stereo_camera(stcam_node->ready,stcam_node->module_enabled, stcam_node->camera_show_EN,
                                       stcam_node->image_data1, stcam_node->image_data2,stcam_node->width,
                                       stcam_node->height, stcam_node->channels,stcam_node->top_trim, stcam_node->bot_trim,
                                       stcam_node->cv_image1,stcam_node->cv_image2 );
                }

            ImGui::EndTabItem();
          }

          if (ImGui::BeginTabItem("Stereo3 Camera ")){
                ImGui::Checkbox("Stereo3 Camera Enable", &stcam_node_tripple->module_enabled);
                stcam_node_tripple->module_enabled=stcam_node_tripple->module_enabled&&stcam_node_tripple->add_module;
                stcam_node_tripple->run();
                ImGui::Checkbox("Show Camera", &stcam_node_tripple->camera_show_EN);

                //mavlink
                ImGui::SetCursorPos(ImVec2(200.0f, 30.0f));
                ImGui::Checkbox("Show Trim", &stcam_node_tripple->show_trimmed);
                ImGui::SetCursorPos(ImVec2(200.0f, 55.0f));
                ImGui::Checkbox("Mavlink", &mvlink_node->mavlink_EN);
                mvlink_node->mavlink_EN=mvlink_node->mavlink_EN&& mvlink_node->add_module;
                mvlink_node->run();
                module_mavlink();




                module_stereo_camera_tripple(stcam_node_tripple->ready,
                                     stcam_node_tripple->module_enabled,
                                     stcam_node_tripple->camera_show_EN,
                                     stcam_node_tripple->image_data1,
                                     stcam_node_tripple->image_data2,
                                     stcam_node_tripple->image_data3,
                                     stcam_node_tripple->image_data4,
                                     stcam_node_tripple->cv_image1,
                                     stcam_node_tripple->cv_image2,
                                     stcam_node_tripple->cv_image3,
                                     stcam_node_tripple->cv_image4,
                                     stcam_node_tripple->width,
                                     stcam_node_tripple->height,
                                     stcam_node_tripple->channels,
                                     stcam_node_tripple->top_trim,
                                     stcam_node_tripple->bot_trim);


            ImGui::EndTabItem();
          }

          ImGui::EndTabBar();
        }
        ImGui::End();

        // status Bar
        ImGui::SetNextWindowPos(ImVec2(viz_param->menu2_windwo_width+viz_param->camera_view_width+10, 20+viz_param->menu1_windwo_height));
        ImGui::SetNextWindowSize(ImVec2(viz_param->status_bar_width, viz_param->camera_view_height));
        ImGui::Begin("Status", nullptr, ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize);

        ImGui::End();

        //imgui render here
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();



    }else{
      /* close the window*/
      /*Terminate the program */
      exit(0);
    }
  }
}

void viz::module_3Dsensor(std::vector<Eigen::Matrix4d,Eigen::aligned_allocator<Eigen::Matrix4d>> pose){

  if(!glfwWindowShouldClose(window)){

      // per-frame time logic
      // --------------------
      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      //We then call processInput every iteration of the render loop:
      processInput(window);
      /* At the start of frame we want to clear the screen*/
      glClearColor(viz_param->bg_color_R, viz_param->bg_color_G, viz_param->bg_color_B, 0.5f);
      glEnable(GL_DEPTH_TEST);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // render here
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
      /*
      ourShader.use();
      // create transformations
      glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
      glm::mat4 view          = glm::mat4(1.0f);
      glm::mat4 projection    = glm::mat4(1.0f);
      model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
      //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
      view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));
      projection = glm::perspective(glm::radians(45.0f), (float)viz_param->width / (float)viz_param->height, 0.1f, 100.0f);
      // retrieve the matrix uniform locations
      unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
      unsigned int viewLoc  = glGetUniformLocation(ourShader.ID, "view");
      // pass them to the shaders (3 different ways)
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
      // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
      ourShader.setMat4("projection", projection);

      */
      // activate shader
      ourShader.use();

      glm::mat4 projection = glm::perspective(glm::radians(fov), (float)viz_param->width / (float)viz_param->height, 0.1f, 100.0f);
      ourShader.setMat4("projection", projection);

      // camera/view transformation
      glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
      float radius = 5.0f;
      float camX   = sin(glfwGetTime()) * radius;
      float camZ   = cos(glfwGetTime()) * radius;
      //view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
      view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
      ourShader.setMat4("view", view);
      glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
      //axis_rotation_angle=(float)glfwGetTime() * glm::radians(50.0f);
      model = glm::rotate(model, axis_rotation_angle, roation_axis);
      ourShader.setMat4("model", model);

      // glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
      // glDrawElements(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, 0);
      //glDrawArrays(GL_TRIANGLES, 0, 3);
      // glBindVertexArray(0); // no need to unbind it every time

      ax.draw_axes();
      _pose.draw_pose(pose);

      // render your IMGUI GUI

      // main menu bar
      int main_menu_h=20;
      if (ImGui::BeginMainMenuBar()) {
          if (ImGui::BeginMenu("File")) {
              ImGui::EndMenu();
          }
          if (ImGui::BeginMenu("Tools")) {
              ImGui::EndMenu();
          }
          ImGui::EndMainMenuBar();
      }

      //////////////////////////////////////////////////////////////////////////
      //left menu
      ImGui::SetNextWindowPos(ImVec2(0, main_menu_h));
      ImGui::SetNextWindowSize(ImVec2(viz_param->menu2_windwo_width, viz_param->menu2_windwo_height-main_menu_h));

      ImGui::Begin("Navigator", nullptr, ImGuiWindowFlags_NoResize);
      ImGui::End();

      //////////////////////////////////////////////////////////////////////////
      //Right menu
      ImGui::SetNextWindowPos(ImVec2(viz_param->width-viz_param->menu_windwo_width, main_menu_h));
      ImGui::SetNextWindowSize(ImVec2(viz_param->menu_windwo_width, viz_param->menu_windwo_height-main_menu_h));

      ImGui::Begin("Control Panel", nullptr, ImGuiWindowFlags_NoResize);
      if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None)){
        if (ImGui::BeginTabItem("General")){

          ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Advanced")){

          ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Parameters")){

          ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
      }
      ImGui::End();

      // Render dear imgui into screen
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      glfwSwapBuffers(window);
      glfwPollEvents();
  }

}
