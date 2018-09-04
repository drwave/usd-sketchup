#include "USDExporter.h"
#include <iostream>
#include <string>

int
main(int argc, const char * argv[]) {
    if (argc < 3) {
        std::cerr   << "USAGE: " << argv[0]
        << " <in.skp> <out.usd[a,z]>" << std::endl;
        return -1;
    }
    const std::string skpFile = std::string(argv[1]);
    const std::string usdFile = std::string(argv[2]);
    
    try {
        auto myExporter = USDExporter();
        myExporter.SetExportToSingleFile(false);
        myExporter.SetExportARKitCompatibleUSDZ(true);
        if (!myExporter.Convert(skpFile, usdFile, NULL)) {
            std::cerr << "Failed to save USD file " << usdFile << std::endl;
            return -2;
        }
    } catch (...) {
        std::cerr << "Failed to save USD file " << usdFile
        << " (Exception was thrown)" << std::endl;
        return -3;
    }
    std::cerr << "Wrote USD file " << usdFile << std::endl;
    return 0;
}

