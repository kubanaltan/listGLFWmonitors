#include <iostream>
// GLFW
#include <GLFW/glfw3.h>

void printModes(const GLFWvidmode* modes, int modeCount)
{
    for (int i = 0; i < modeCount; ++i )
    {
        std::cout << "Mode #" << i << ": ";
        std::cout << modes[i].width << "x";
        std::cout << modes[i].height <<"_";
        std::cout << modes[i].refreshRate << "Hz ";
        std::cout << "R" << modes[i].redBits;
        std::cout << "G" << modes[i].greenBits;
        std::cout << "B" << modes[i].blueBits;
        
        std::cout << std::endl;
    }
}

int main()
{
    // Init GLFW
    glfwInit();
    
    int monitorCount;
    GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
    std::cout << "Number of detected monitors: " << monitorCount << std::endl;
    
    int widthMM, heightMM;
    int modeCount;
    
    // Dump information for all the monitors detected by GLFW in this loop
    for ( int i=0; i<monitorCount; ++i)
    {
        glfwGetMonitorPhysicalSize(monitors[i], &widthMM, &heightMM);
        std::cout << "Monitor#" << i << " Physical Size: " << widthMM << "mm. x " << heightMM << "mm." << std::endl;
        
        // Get information for monitor #i in terms of modes and count of modes
        const GLFWvidmode* modes = glfwGetVideoModes(monitors[i], &modeCount);
        std::cout << "Monitor#" << i << " Supported Video Modes: " << modeCount << std::endl;
        
        printModes(modes, modeCount);
    }
    
    // Terminates GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}