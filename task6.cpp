#include <iostream>
#include <string>
#include <vector>

struct CheckPoint
{
    std::string name;
    float latitude;
    float longitude;
    float penalty;

    CheckPoint(std::string name, 
                float latitude, 
                float longitude, 
                float penalty,
                std::string message): 
                                    name(name), 
                                    latitude(latitude),
                                    longitude(longitude),
                                    penalty(penalty),
                                    message(message) {};

    void printMessage(){std::cout << message;}
    void setMessage(std::string text){message=text;};

    private:
        std::string message;

};

class CheckPointBuilder
{
    public:
    virtual CheckPoint* BuildCheckPoint(std::string name, 
                                        float latitude,
                                        float longitude,
                                        float penalty)=0;
};

class ObligatoryCheckPointsBuilder: public CheckPointBuilder
{
    public:
    CheckPoint* BuildCheckPoint(std::string name, 
                                float latitude,
                                float longitude,
                                float penalty) override
    {
        std::string message = "Obligatory CheckPoint <<" +
                               name + ">>" +
                               " on coordinates (" + 
                               std::to_string(latitude) + 
                               "," +
                               std::to_string(longitude) +
                               "). Failed SU.\n";
        CheckPoint *obligatory_checkPoint = new CheckPoint(name, 
                                                          latitude, 
                                                          longitude, 
                                                          0,
                                                          message);
        return obligatory_checkPoint;
    }
};

class NotObligatoryCheckPointsBuilder: public CheckPointBuilder
{
    public:
    CheckPoint* BuildCheckPoint(std::string name, 
                                float latitude,
                                float longitude,
                                float penalty) override
    {
        std::string message = "Not Obligatory CheckPoint <<" + 
                                name + ">>" +
                               " on coordinates (" + 
                               std::to_string(latitude) + 
                               "," +
                               std::to_string(longitude) +
                               "). Penalty " + 
                               std::to_string(penalty)
                               +"\n";
        CheckPoint* notObligatory_checkPoint = new CheckPoint(name, 
                                                              latitude, 
                                                              longitude, 
                                                              penalty,
                                                              message);
        return notObligatory_checkPoint;
    }
};

class TraceDirector
{
    public:
        TraceDirector(CheckPointBuilder* newBuilder): builder(newBuilder){};
        void setBuilder(CheckPointBuilder* newBuilder){builder=newBuilder;};
        void addCheckPoint(std::string name, 
                            float latitude,
                            float longitude,
                            float penalty=0)
        {
            CheckPoint* cp = builder->BuildCheckPoint(name, latitude, longitude, penalty);
            trace.push_back(cp);
        }
        float getSumPenalty()
        {
            float sum;
            for(auto checkPoint: trace)
                sum+=checkPoint->penalty;
            return sum;
        }
        void aboutTrace()
        {
            for(auto checkPoint: trace)
                checkPoint->printMessage();
        }

    private:
        CheckPointBuilder* builder;
        std::vector<CheckPoint*> trace;
};


int main(){
    CheckPointBuilder* o_builder = new ObligatoryCheckPointsBuilder;
    TraceDirector director = TraceDirector(o_builder);
    director.addCheckPoint("Start", 60.002, 30.013);
    director.addCheckPoint("first stop", 60.402, 30.113, 122.0);

    CheckPointBuilder* no_builder = new NotObligatoryCheckPointsBuilder;
    director.setBuilder(no_builder);
    director.addCheckPoint("half of the way", 62.56, 36.467, 1000);
    director.addCheckPoint("finish", 65.015, 40.89, 500);

    std::cout<< "Sum penalty: " << std::to_string(director.getSumPenalty())<<"\n";
    director.aboutTrace();
    return 0;
}