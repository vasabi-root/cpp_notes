#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <vector>
#include <set>

using std::cout;
using std::cin;
using std::endl;

using std::vector;

class UserID {
private:
    int id = 0;
public:
    UserID(int id): id(id) {}
    explicit operator int() {
        return id;
    }

    friend std::ostream& operator <<(std::ostream&, const UserID&);
};

class GroupID {
private:
    int id = 0;
public:
    GroupID(int id): id(id) {}

    explicit operator int() {
        return id;
    }

    friend std::ostream& operator << (std::ostream&, const GroupID&);
};

std::ostream& operator << (std::ostream& out, const UserID& uid) {
    cout << "Uid: " << uid.id << endl;
    return out;
}

std::ostream& operator << (std::ostream& out, const GroupID& gid) {
    cout << "Gid: " << gid.id << endl;
    return out;
}

template <typename T>
std::ostream& operator << (std::ostream &out, const vector<T> &v) {
    out << "{";
    for (T x: v) {
        // out << i << " ";
        out << " " << x;
    }
    out << " }" << endl;
    return out;
}

UserID operator ""_uid(uint64_t x) {
    return UserID(x);
}

struct GreaterThanZero {
    bool operator () (const int64_t &x) {
        return x > 0;
    }
};
struct AbsCompare {
    bool operator () (const int &x, const int &y) {
        return abs(x) < abs(y);
    }
};

int main() {
    {
        UserID uid = 5_uid;
        GroupID gid = 10;
        int a = 10;
        int *pa = &a, *b = pa;

        cout << uid;
        cout << gid;

        // cout << uid + gid; // без explicit работает, а не должно
    }
    {
        std::vector<int64_t> v{-1, 2, 3, 4, 5, -1, -3, 0};
        GreaterThanZero f;
        cout << *std::find_if(v.begin(), v.end(), f) << endl;
        cout << *std::find_if(v.begin(), v.end(), GreaterThanZero()) << endl;

        std::sort(v.begin(), v.end(), AbsCompare());
        cout << v;

        std::set<int64_t, AbsCompare> s;
        s.insert(3);
        s.insert(2);
        s.insert(1);

        // std::sort(s.begin(), s.end(), std::greater<int>());
        for (auto x: s) {
            cout << x << " ";
        }
        cout << endl;

    }
    
    return 0;
}
