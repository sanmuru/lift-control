// LiftControl.Console.Win32.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include "Program.h"
#include "Lift.h"

using namespace std;
using namespace LiftControl;

static wstring Trim(wstring str, bool trim_left = true, bool trim_right = true)
{
    auto begin = str.cbegin();
    if (trim_left) begin = find_if_not(begin, str.cend(), [](wchar_t wc) { return isspace(wc); });
    auto end = str.crend();
    if (trim_right) end = find_if_not(str.crbegin(), str.crend(), [](wchar_t wc) { return isspace(wc); });
    return wstring(begin, end.base());
}

static bool TryParseInt(wstring str, int& result)
{
    str = Trim(str);
    size_t end;
    auto value = std::stoi(str, &end);
    if (end == str.size())
    {
        result = value;
        return true;
    }
    else
        return false;
}

static bool TryParseGenerationArg(wstring arg, int& result)
{
    wstring switch_str = L"/g:";
    auto switch_len = switch_str.size();
    if (!arg.starts_with(switch_str))
        return false;

    return TryParseInt(arg.substr(switch_len, arg.size() - switch_len), result);
}

static vector<wstring> SplitString(wstring str, bool remove_empty_entry = false)
{
    vector<wstring> result;
    wistringstream iss(str);
    wstring s;
    while (iss >> s)
    {
        if (remove_empty_entry && s.empty())
            continue;
        result.push_back(s);
    }
    return result;
}

int wmain(int argc, wchar_t* argv[])
{
    int g;
    if (argc != 1 &&
        TryParseGenerationArg(argv[1], g))
    {
        switch (g)
        {
        case 1: return RunGeneration1();
        }
    }

    wcout << L"expected argument /g:< generation_num >.";
    return HostErrorCode;
}

int LiftControl::RunGeneration1()
{
    wstring line;

    getline(wcin, line);
    int at_floor;
    if (!TryParseInt(line.data(), at_floor))
    {
        assert(false); // wrong format for lift initial position.
        return HostErrorCode;
    }

    auto lift = new LiftControl::Generation1::Lift(at_floor);

    while (true)
    {
        getline(wcin, line);
        if (line.empty() || all_of(line.cbegin(), line.cend(), [](wchar_t wc) { return iswspace(wc); }))
        {
            break;
        }
        
        auto args = SplitString(line, true);
        int from_floor, to_floor;
        if (args.size() == 2 &&
            TryParseInt(args[0], from_floor) &&
            TryParseInt(args[1], to_floor))
        {
            lift->add(from_floor, to_floor);
        }
        else
        {
            assert(false); // wrong format for lift guest task.
            return HostErrorCode;
        }
    }

    bool flag = true;
    for (auto floor : lift->get_schedule())
    {
        if (flag)
            flag = false;
        else
            wcout << L' ';

        wcout << floor;
    }
    return 0;
}
