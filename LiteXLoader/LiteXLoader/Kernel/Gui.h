#include <string>
#include <vector>

int Raw_SendSimpleForm(Player* player,const std::string &title,
    const std::string &content,const std::vector<std::string> &buttons, const std::vector<std::string>& images);

int Raw_SendModalForm(Player* player,const std::string &title,
    const std::string &content,const std::string &button1,const std::string &button2);

int Raw_SendRawForm(Player* player, const std::string& data);
int Raw_SendCustomForm(Player* player,const std::string &data);