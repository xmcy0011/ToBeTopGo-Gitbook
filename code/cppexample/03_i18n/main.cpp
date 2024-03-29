#include "cpptoml.h"

#include <iostream>
#include <memory>
#include <string>

#include "http_rest_exception.h"
#include "i18n_res_loader.h"

const std::string kSomeError = "efast.err.in.approval";
const std::string enUSLan = "en-US";
const std::string zhCNLan = "zh-CN";

void testConflictExecption() { throw ConflictException(__FILE__, __LINE__, "eofs", kSomeError); }

void testLoadFile() {
    std::string enUSFile = "../locale/en-US.toml";
    std::string zhCNFile = "../locale/zh-CN.toml";

    auto &localizer = I18NResLoader::GetInstance();
    localizer.LoadFile(enUSLan, enUSFile);
    localizer.LoadFile(zhCNLan, zhCNFile);

    std::cout << "en-US: " << localizer.Localize(enUSLan, kSomeError) << std::endl;
    std::cout << "zn-CH: " << localizer.Localize(zhCNLan, kSomeError) << std::endl;

    std::string resourceNotFoundKey = "efast.err.in.approvalxxxxxxxxxxxx";
    std::cout << "en-US: " << localizer.Localize(enUSLan, resourceNotFoundKey) << std::endl;
    std::cout << "zn-CH: " << localizer.Localize(zhCNLan, resourceNotFoundKey) << std::endl;
}

void testLoadDir() {
    auto &localizer = I18NResLoader::GetInstance();
    localizer.LoadDir("../locale");

    std::cout << "en-US: " << localizer.Localize(enUSLan, kSomeError) << std::endl;
    std::cout << "zn-CH: " << localizer.Localize(zhCNLan, kSomeError) << std::endl;
}

/**
 * @brief ../a/a.test:314 -> a.test:314
 * @return std::string 
 */
std::string getFileName(const std::string &filePath) {
    size_t pos = filePath.find_last_of('/');
    std::string name = filePath;
    if (pos != std::string::npos) {
        name = filePath.substr(pos + 1, name.length());
    }
    return name;
}

int main(int argc, char **argv) {
    testLoadFile();
    testLoadDir();

    std::cout << getFileName("../a/a.test:314") << std::endl;

    try {
        testConflictExecption();
    } catch (ClientException e) {
        std::cout << "client execption: " << e.GetResouceKey() << std::endl;
    } catch (ServerException e) {
        std::cout << "client execption: " << e.GetResouceKey() << std::endl;
    }

    return 0;
}
