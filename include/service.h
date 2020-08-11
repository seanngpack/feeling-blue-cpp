#ifndef FEELING_BLUE_SERVICE_H
#define FEELING_BLUE_SERVICE_H


#include <string>

namespace bluetooth {

    namespace wrapper {
        class Wrapper;
    }

    class Service {
    public:

        Service(std::string UUID, std::shared_ptr<wrapper::Wrapper> bt);

        ~Service();


    private:
        struct ServiceImpl;
        ServiceImpl *sImpl;
    };
}

#endif //FEELING_BLUE_SERVICE_H
