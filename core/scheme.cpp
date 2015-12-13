#include "scheme.h"

void Scheme::MirrorY()
{
    for (int i = 0; i < size.x / 2; ++i)
        for (int j = 0; j < size.y; ++j)
            std::swap(data[j][i], data[j][static_cast<size_t>(size.x) - i - 1]);
}

void Scheme::MirrorX()
{
    for(int y = 0; y < size.y / 2; ++y)
    {
      for(int x = 0; x < size.x; ++x)
      {
        std::swap(data[y][x], data[static_cast<size_t>(size.y) - y - 1][x]);
      }
    }
}

void Scheme::RotateCCW()
{
    MirrorY();
    Transpose();

}

void Scheme::RotateCW()
{
    MirrorX();
    Transpose();

}

void Scheme::Transpose()
{
    if(size.x == size.y)
    {
        for (int i = 0; i < size.x; ++i)
            for (int j = i; j < size.y; ++j)
                std::swap(data[j][i], data[i][j]);
    }
    else
    {
        LetterMatrix a;
        a.resize(static_cast<size_t>(size.x));
        for(auto &row : a)
            row.resize(static_cast<size_t>(size.y));

        for (int i = 0; i < size.x; ++i)
            for (int j = 0; j < size.y; ++j)
                a[i][j] = data[j][i];
        data = a;
        size = {size.y, size.x};
    }
}

void Scheme::Resize(glm::vec2 __size)
{
    data.clear();
    data.resize(static_cast<size_t>(size.y));
    for(auto &row : data)
        row.resize(static_cast<size_t>(size.x));
    size = __size;
}

void Scheme::LogData() const
{
    for (int j = 0; j < size.y; ++j)
    {
        std::string s;
        for (int i = 0; i < size.x; ++i)
        {
            s += data[j][i];
        }
        LOG(info) << s;
    }
}

void Scheme::Set(std::vector<std::vector<Letter>> __data)
{
    assert(data.size() == 0 && "empty matrix");
    data.clear();
    size = {__data[0].size(), __data.size()};
    data = __data;
}

void Scheme::NumericTransform(int __num)
{
    auto a = __num % TRANSFORM_AFTER_LAST;
    switch (a)
    {
        case TRANSFORM_MX:
        MirrorX();
        break;
        case TRANSFORM_MY:
        MirrorY();
        break;
        case TRANSFORM_CCW:
        RotateCCW();
        break;
        case TRANSFORM_CW:
        RotateCCW();
        break;
        case TRANSFORM_T:
        RotateCCW();
        break;
    }
}

bool vec2ParseHelper(rapidjson::Value &__val, glm::vec2 &vec)
{
    if(!__val.IsArray() || __val.Size() != 2)
        return false;
    vec = {__val.Begin()->GetInt(), (__val.Begin() + 1)->GetInt()};
    return true;
}

bool Scheme::Deserialize(rapidjson::Value &__val)
{
    Scheme &s = *this;
    if(!__val.HasMember("building"))
    {
        LOG(error) << "scheme has no \"building\"";
        return false;

    }
    s.type = __val["building"].GetString();

    if(!__val.HasMember("dictionary"))
    {
        LOG(error) << "scheme has no \"dictionary\"";
        return false;
    }
    rapidjson::Value &dict = __val["dictionary"];
    if(dict.IsArray())
    {
        for(int j=0; j < dict.Size(); j++)
        {
            rapidjson::Value &pair = dict[j];
            s.dict[pair.Begin()->GetString()[0]] = (pair.Begin() + 1)->GetString();
        }
    }
        else
    {
        LOG(error) << "scheme has broken \"dictionary\" (must be [[\"i\",\"id_string\"], ...])";
        return false;
    }

    if(!__val.HasMember("size"))
    {
        LOG(error) << "scheme has no \"size\"";
        return false;
    }
    rapidjson::Value &size = __val["size"];
    if(!vec2ParseHelper(size, s.size))
    {
        LOG(error) << "scheme has broken \"size\" (must be [x,y])";
        return false;
    }
    s.Resize(s.size);

    if(!__val.HasMember("data"))
    {
        LOG(error) << "scheme has no \"data\"";
        return false;
    }
    rapidjson::Value &data = __val["data"];
    if(data.IsArray())
    {
        if(data.Size() != s.size.y)
        {
            LOG(error) << "scheme has broken \"data\", height is " <<
                          data.Size() << ", scheme has height " << s.size.y;
            return false;
        }
        for(size_t j = 0; j < data.Size(); j++)
        {
            auto str = data[j].GetString();
            if(data[j].GetStringLength() != s.size.x)
            {
                LOG(error) << "scheme has broken \"data\", line " << j <<
                              " lenght is " << data[j].GetStringLength() << ", scheme has lenght " << s.size.x;
                return false;
            }
            for(size_t k = 0; k < data[j].GetStringLength(); ++k)
            {
                s.data[j][k] = str[k];
            }
        }
    }
    else
    {
        LOG(error) << "scheme has broken \"data\" (must be [\"   abcd   \", ...])";
        return false;
    }

    //test
    bool broken = false;
    for(int m = 0; m < s.size.x && !broken; m++)
        for(int n = 0; n < s.size.y && !broken; n++){
            if(s.data[n][m] == '.') continue;
            auto it = s.dict.find(s.data[n][m]);
            if(it == s.dict.end())
            {
                broken = true;
                LOG(error) << "\"" << s.data[n][m] << "\" not found in dictionary";
            }
        }
    if(broken) {
        return false;
    }
    return true;
}
