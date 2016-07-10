#include <iostream>
#include <string>
// GLFW
#include <GLFW/glfw3.h>

namespace glfw
{
    class System
    {
    private:
        GLFWmonitor* primaryMonitor {nullptr};
        GLFWmonitor** monitors {nullptr};
        int monitorCount {-1};
        
    public:
        System()
        {
            primaryMonitor = glfwGetPrimaryMonitor();
            monitors = glfwGetMonitors(&monitorCount);
        }
        
        int GetMonitorCount()
        {
            return monitorCount;
        }
        
        GLFWmonitor* GetPrimaryMonitor()
        {
            return primaryMonitor;
        }
        
        GLFWmonitor* GetMonitor(int monitorNumber)
        {
            if (monitorNumber < monitorCount)
            {
                return monitors[monitorNumber];
            }
            else
            {
                return nullptr;
            }
        }
    };
    
    class Monitor
    {
    private:
        
        bool isInitialized {false};
        bool isPrimary {false};
        
        const char* monitorName {nullptr};
        const GLFWgammaramp* orgRamp {nullptr};
        const GLFWvidmode* availableModes {nullptr};
        const GLFWvidmode* currentMode {nullptr};
        
        int availableModeCount {-1};
        int widthMM {-1}, heightMM {-1};
        int xpos {-1}, ypos {-1};
        
        void PrintRamp(const GLFWgammaramp* ramp)
        {
            for (int i = 0; i < ramp->size; ++i)
            {
                std::cout << ramp->red[i] << ",";
                std::cout << ramp->green[i] << ",";
                std::cout << ramp->blue[i];
                
                std::cout << std::endl;
            }
        }
        
        std::string ConvertModeToString(const GLFWvidmode* mode)
        {
            std::string modeString {""};
            
            if (mode)
            {
                // Generate current mode as string
                modeString.append(std::to_string(mode->width));
                modeString.append("x");
                modeString.append(std::to_string(mode->height));
                modeString.append("_");
                modeString.append(std::to_string(mode->refreshRate));
                modeString.append("Hz ");
                modeString.append("R");
                modeString.append(std::to_string(mode->redBits));
                modeString.append("G");
                modeString.append(std::to_string(mode->greenBits));
                modeString.append("B");
                modeString.append(std::to_string(mode->blueBits));
            }
            
            return modeString;
        }
        
    public:
        
        bool Init(int monitorNumber)
        {
            System mySystem;
            GLFWmonitor* monitor = mySystem.GetMonitor(monitorNumber);
            
            if (monitor)
            {
                if (monitor == mySystem.GetPrimaryMonitor()) {isPrimary = true;}
                
                monitorName = glfwGetMonitorName(monitor);
                if (monitorName == nullptr) {return false;}
                
                orgRamp = glfwGetGammaRamp(monitor);
                if (orgRamp == nullptr) {return false;}
                
                glfwGetMonitorPos(monitor, &xpos, &ypos);
                glfwGetMonitorPhysicalSize(monitor, &widthMM, &heightMM);
                
                availableModes = glfwGetVideoModes(monitor, &availableModeCount);
                
                currentMode = glfwGetVideoMode(monitor);
                if (currentMode == nullptr) {return false;}
                
                isInitialized = true;
                return true;
            }
            else
            {
                return false;
            }
        }
        
        bool IsPrimary()
        {
            return isPrimary;
        }
        
        void PrintOriginalRamp()
        {
            if (isInitialized)
            {
                PrintRamp(orgRamp);
            }
            else
            {
                std::cout << "Monitor Gamma not initialized." << std::endl;
            }
        }
        
        const char* GetMonitorName()
        {
            if (isInitialized)
            {
                return monitorName;
            }
            else
            {
                return "NOT AVAILABLE";
            }
        }
        
        int GetMonitorPhysicalSizeX()
        {
            return widthMM;
        }
        
        int GetMonitorPhysicalSizeY()
        {
            return heightMM;
        }
        
        int GetMonitorPositionX()
        {
            return xpos;
        }
        
        int GetMonitorPositionY()
        {
            return ypos;
        }
        
        int GetAvailableModeCount()
        {
            return availableModeCount;
        }
        
        const GLFWvidmode* GetCurrentVideoMode()
        {
            return currentMode;
        }
        
        std::string GetCurrentVideoModeString()
        {
            return ConvertModeToString(currentMode);
        }
        
        const GLFWvidmode* GetVideoMode(int mode)
        {
            if (mode < availableModeCount)
            {
                return &availableModes[mode];
            }
            else
            {
                return nullptr;
            }
        }
        
        std::string GetVideoModeString(int mode)
        {
            if (mode < availableModeCount)
            {
                return ConvertModeToString(&availableModes[mode]);
            }
            else
            {
                return "";
            }
        }
        
    };
}

int main(int argc, const char * argv[])
{
    // set monitorNumber to 0 in case we don't receive any argument
    int monitorNumber{0};
    
    // Parse first parameter to desired monitor number
    if (argc > 1)
    {
        monitorNumber = static_cast<int>(strtoull(argv[1], nullptr, 10));
    }
    
    // Init GLFW
    glfwInit();
    
    glfw::System mySystem;
    glfw::Monitor myMonitor;
    
    if (myMonitor.Init(monitorNumber))
    {
        // Print original LUT loaded to this monitor
        myMonitor.PrintOriginalRamp();
        
        std::cout << "Monitor name: " << myMonitor.GetMonitorName();
        if (myMonitor.IsPrimary()) { std::cout << " (primary)";}
        std::cout << std::endl;
        
        // Print currently active video mode
        std::cout << "Current video mode: " << myMonitor.GetCurrentVideoModeString() << std::endl;
        
        // Print all available video modes
        for (int i=0; i<myMonitor.GetAvailableModeCount(); ++i)
        {
            std::cout << "Video mode #" << i <<": "<< myMonitor.GetVideoModeString(i) << std::endl;
        }
        
        // Print Monitor Physical Size in mm.
        std::cout << "Physical Size: ";
        std::cout << myMonitor.GetMonitorPhysicalSizeX() << "mm. x ";
        std::cout << myMonitor.GetMonitorPhysicalSizeY() << "mm." << std::endl;
        
        std::cout << "Desktop Position Offset: ";
        std::cout << myMonitor.GetMonitorPositionX() << ",";
        std::cout << myMonitor.GetMonitorPositionY() << std::endl;
    }
    else
    {
        std::cout << "Monitor #" << monitorNumber << " not available." << std::endl;
    }
    
    std::cout << "Total Monitors: "<< mySystem.GetMonitorCount() << std::endl;
    
    // Terminates GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    
    return 0;
}