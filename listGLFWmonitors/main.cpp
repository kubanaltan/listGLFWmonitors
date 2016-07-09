#include <iostream>
// GLFW
#include <GLFW/glfw3.h>

void printSingleMode(const GLFWvidmode mode)
{
    std::cout << mode.width << "x";
    std::cout << mode.height <<"_";
    std::cout << mode.refreshRate << "Hz ";
    std::cout << "R" << mode.redBits;
    std::cout << "G" << mode.greenBits;
    std::cout << "B" << mode.blueBits;
    
    std::cout << std::endl;
}

void printModeArray(const GLFWvidmode* modes, int modeCount)
{
    for (int i = 0; i < modeCount; ++i )
    {
        std::cout << "Mode #" << i << ": ";
        printSingleMode(modes[i]);

    }
}

int main()
{
    // Init GLFW
    glfwInit();
    
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    
    int monitorCount;
    GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
    std::cout << "Number of detected monitors: " << monitorCount << std::endl;
    
    int widthMM, heightMM;
    int xpos, ypos;
    int modeCount;
    const char* monitorName;
    
    // Dump information for all the monitors detected by GLFW in this loop
    for ( int i=0; i < monitorCount; ++i)
    {
        monitorName = glfwGetMonitorName(monitors[i]);
        std::cout << "Monitor#" << i << " Name: " << monitorName;

        // Indicate if this monitor is the "primary" monitor
        if (monitors[i] == primaryMonitor)
        {
            std::cout << " (primary)";
        }
        std::cout << std::endl;
        
        // Get Monitors Desktop Position, the way it has been offset logically
        glfwGetMonitorPos(monitors[i], &xpos, &ypos);
        std::cout << "Monitor#" << i << " Position: " << xpos << "," << ypos << std::endl;
        
        glfwGetMonitorPhysicalSize(monitors[i], &widthMM, &heightMM);
        std::cout << "Monitor#" << i << " Physical Size: " << widthMM << "mm. x " << heightMM << "mm." << std::endl;

        // Get currently active video mode for the current monitor
        const GLFWvidmode* currentMode = glfwGetVideoMode(monitors[i]);
        std::cout << "Monitor#" << i << " Active Mode: ";
        printSingleMode(*currentMode);

        // Get information for monitor #i in terms of modes and count of modes
        const GLFWvidmode* modes = glfwGetVideoModes(monitors[i], &modeCount);
        std::cout << "Monitor#" << i << " Supported Video Modes: " << modeCount << std::endl;
        
        printModeArray(modes, modeCount);
    }
    
    // Terminates GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}