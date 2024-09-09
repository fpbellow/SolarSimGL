# SolarSimGL

A simplified simulation of the Sol system in OpenGL along with the use of the Assimp model loading library. The main goal of the project is to provide a method for exploring advanced techniques and topics of OpenGL such as advanced lighting and post processing.

![ezgif-1-bbed928061](https://github.com/user-attachments/assets/b53c6118-13af-47f6-b87e-b709b8fa2f6c)
![Screenshot 2024-09-09 104513](https://github.com/user-attachments/assets/24942511-e2ba-4e22-9dd7-92be6faa4c89)

Current goals:
- resolve issue with framebuffer when window size changes
- fix color adjustment on Earth and its atmosphere due to the bloom pass making such slightly desaturated
- resolve issue with Framebuffer class so the FBOs do not need to be hardcoded in Main
- then finish importing remaining planetary bodies and begin configuring their orbits and rotations

Contributions:
Assets and textures gathered from - https://www.solarsystemscope.com/textures/
