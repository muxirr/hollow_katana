#ifndef _ALTAS_H_
#define _ALTAS_H_

#include <vector>
#include <graphics.h>

class Atlas
{
public:
    Atlas() = default;
    ~Atlas() = default;

    void load(LPCTSTR path_template, int num)
    {
        this->imgList.clear();
        this->imgList.resize(num);

        TCHAR path[256];
        for (int i = 0; i < num; i++)
        {
            _stprintf_s(path, path_template, i + 1);
            loadimage(&imgList[i], path);
        }
    }

    void clear()
    {
        this->imgList.clear();
    }

    int getSize()
    {
        return this->imgList.size();
    }

    IMAGE *getImage(int index)
    {
        if (index < 0 || index >= imgList.size())
            return nullptr;
        return &this->imgList[index];
    }
    void addImage(const IMAGE &img)
    {
        this->imgList.push_back(img);
    }

private:
    std::vector<IMAGE> imgList;
};

#endif // _ALTAS_H_