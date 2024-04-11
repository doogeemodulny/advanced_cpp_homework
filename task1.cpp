#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

class Group {
public:
    Group(int groupId) : groupId(groupId) {}

    void addUser(int userId) {
        users.push_back(userId);
    }
    int getGroupId() const {
        return groupId;
    }
    const std::vector<int>& getUsersList() const {
        return users;
    }

private:
    int groupId;
    std::vector<int> users;  // Список идентификаторов пользователей, состоящих в группе
};

class User {
public:
    User(int userId, std::string username, std::string additionalInfo) 
        : userId(userId), username(username), additionalInfo(additionalInfo), group(nullptr) {}
    User(int userId, std::string username, std::string additionalInfo, Group* group) 
        : userId(userId), username(username), additionalInfo(additionalInfo), group(group) {
        if (group) {
            group->addUser(userId);
        }
    }
    void setGroup(Group* newGroup) {
        group = newGroup;
        if (group) {
            group->addUser(userId);
        }
    }
    void getInfo() const {
        std::cout << "User id is: " << userId << "\n";
        std::cout << "User name is: " << username << "\n";
        std::cout << "User info is: " << additionalInfo << "\n";
        if (group) {
            std::cout << "This User is a member of group " << group->getGroupId() << "\n";
        }
        std::cout << "\n";
    }

private:
    int userId;
    std::string username;
    std::string additionalInfo;
    Group* group;  // ссылка на группу, в которой состоит пользователь
};

class UserManagementSystem {
public:
    void createUser(int userId, std::string username, std::string additionalInfo) {
        User* user = new User(userId, username, additionalInfo);
        allUsersList.insert(std::make_pair(userId, user));
    }
    void createUser(int userId, std::string username, std::string additionalInfo, int groupId) {
        Group* group = allGroupsList.at(groupId);
        User* user = new User(userId, username, additionalInfo, group);
        allUsersList.insert(std::make_pair(userId, user));
    }

    void getUser(int userId)
    {
        User* user = allUsersList.at(userId);
        user->getInfo();
    }

    void deleteUser(int userId) {
        auto it = allUsersList.find(userId);
        if (it != allUsersList.end()) {
            delete it->second;
            allUsersList.erase(it);
        }
    }

    void allUsers() const {
        for (const auto& user_pair : allUsersList) {
            user_pair.second->getInfo();
        }
    }

    void createGroup(int groupId) {
        Group* group = new Group(groupId);
        allGroupsList.insert(std::make_pair(groupId, group)); 
    }

    void getGroup(int groupId)
    {
        Group* group = allGroupsList.at(groupId);
        std::cout<<"Users of this group: \n";
        const std::vector<int>& usersList = group->getUsersList();
            for (int userId : usersList) {
                auto userIt = allUsersList.find(userId);
                if (userIt != allUsersList.end()) {
                    userIt->second->getInfo();
                }
            }
    }

    void deleteGroup(int groupId) {
        auto it = allGroupsList.find(groupId);
        if (it != allGroupsList.end()) {
            delete it->second;
            allGroupsList.erase(it);
        }
    }

    void allGroups() const {
        for (const auto& group_pair : allGroupsList) {
            std::cout << "Group id is " << group_pair.first << "\n";
            const std::vector<int>& usersList = group_pair.second->getUsersList();
            for (int userId : usersList) {
                auto userIt = allUsersList.find(userId);
                if (userIt != allUsersList.end()) {
                    userIt->second->getInfo();
                }
            }
        }
    }
~UserManagementSystem() {
        for (const auto& user_pair : allUsersList) {
            delete user_pair.second;
        }
        for (const auto& group_pair : allGroupsList) {
            delete group_pair.second;
        }
    }
private:
    std::map<int, User*> allUsersList;  // Список всех пользователей
    std::map<int, Group*> allGroupsList;  // Список всех групп
};


int main() {
    UserManagementSystem system;

    std::string choice;
    bool exit = false;

    std::cout << "\n\n\nList of allowed commands:\n";
    std::cout << "createUser <userId> <userName> <additionalInfo>\n";
    std::cout << "deleteUser <userId>\n";
    std::cout << "allUsers\n";
    std::cout << "getUser <userId>\n";
    std::cout << "createGroup <groupId>\n";
    std::cout << "deleteGroup <groupId>\n";
    std::cout << "allGroups\n";
    std::cout << "getGroup <groupId>\n";
    std::cout << "help\n";
    std::cout << "exit\n";
    std::cout << "\nEnter your choice: \n";

    while (!exit) {
        std::getline(std::cin, choice);

        std::vector<std::string> choice_splitted;
        std::stringstream ss(choice);
        std::string token;

        while (ss >> token) {
            choice_splitted.push_back(token);
        }

        // createUser
        if (choice_splitted[0]=="createUser")
        {
            try {
                int userId = std::stoi(choice_splitted[1]);
                std::string userName = choice_splitted[2];
                std::string additionalInfo = choice_splitted[3];
                if(choice_splitted.size() == 5){
                    int groupId = std::stoi(choice_splitted[4]);
                    system.createUser(userId, userName, additionalInfo, groupId);
                    }

                system.createUser(userId, userName, additionalInfo);

            } 
            catch (const std::out_of_range&) {
                std::cout << "\ntoo many arguments or group does not exist\n\n" << std::endl;
            }
            catch (const std::invalid_argument&) {
                std::cout << choice_splitted[1] << "\ninvalid argument type\n\n" << std::endl;
            }

        }

        //all users
        else if (choice_splitted[0]=="allUsers")
        {
            system.allUsers();
        }

        // getUser
        else if (choice_splitted[0]=="getUser")
        {
            try {
                int userId = std::stoi(choice_splitted[1]);
                system.getUser(userId);

            } 
            catch (const std::out_of_range&) {
                std::cout << "\ntoo many arguments or user does not exist\n\n" << std::endl;
            }
            catch (const std::invalid_argument&) {
                std::cout << choice_splitted[1] << "\ninvalid argument type\n\n" << std::endl;
            }
        }


        // deleteUser
        else if (choice_splitted[0]=="deleteUser")
        {
             try {
                int userId = std::stoi(choice_splitted[1]);
                system.deleteUser(userId);
            } 
            catch (const std::out_of_range&) {
                std::cout << "\ntoo many arguments or user does not exist\n\n" << std::endl;
            }
            catch (const std::invalid_argument&) {
                std::cout << choice_splitted[1] << "\ninvalid argument type\n\n" << std::endl;
            }
        }


        //all groups
        else if (choice_splitted[0]=="allGroups")
        {
            system.allGroups();
        }


        //createGroup
        else if (choice_splitted[0]=="createGroup")
        {
            try {
                int groupId = std::stoi(choice_splitted[1]);
                system.createGroup(groupId);

            } 
            catch (const std::out_of_range&) {
                std::cout << "\ntoo many arguments\n\n" << std::endl;
            }
            catch (const std::invalid_argument&) {
                std::cout << choice_splitted[1] << "\ninvalid argument type\n\n" << std::endl;
            }
        }

        // getGroup
        else if (choice_splitted[0]=="getGroup")
        {
            try {
                int groupId = std::stoi(choice_splitted[1]);
                system.getGroup(groupId);

            } 
            catch (const std::out_of_range&) {
                std::cout << "\ntoo many arguments or group does not exist\n\n" << std::endl;
            }
            catch (const std::invalid_argument&) {
                std::cout << choice_splitted[1] << "\ninvalid argument type\n\n" << std::endl;
            }
        }

        //deleteGroup
        else if (choice_splitted[0]=="deleteGroup")
        {
            try {
                int groupId = std::stoi(choice_splitted[1]);
                system.deleteGroup(groupId);

            } 
            catch (const std::out_of_range&) {
                std::cout << "\ntoo many arguments\n\n" << std::endl;
            }
            catch (const std::invalid_argument&) {
                std::cout << choice_splitted[1] << "\ninvalid argument type\n\n" << std::endl;
            }
        }


        else if (choice == "help"){
            std::cout << "\n\n\nList of allowed commands:\n";
            std::cout << "createUser <userId> <userName> <additionalInfo>\n";
            std::cout << "deleteUser <userId>\n";
            std::cout << "allUsers\n";
            std::cout << "getUser <userId>\n";
            std::cout << "createGroup <groupId>\n";
            std::cout << "deleteGroup <groupId>\n";
            std::cout << "allGroups\n";
            std::cout << "getGroup <groupId>\n";
            std::cout << "help\n";
            std::cout << "exit\n";
            std::cout << "\nEnter your choice: \n\n";
        }
        else if (choice == "exit") {
            exit = true;
        }

        else
        {
            std::cout<<"\ntype command >>help to see the list of allowed commands\n\n";
        }
    }

    return 0;
}




