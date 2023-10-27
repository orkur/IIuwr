#include <iostream>
#include <memory>
#include <random>

constexpr auto n{20u};
constexpr auto m{40u};

class Counter
{
public:
    uint64_t get() { return value; }
    void add(uint64_t x) { value += x; }

    virtual ~Counter()
    {
        std::cerr << value << std::endl;
    }

private:
    uint64_t value{1ull};
};

using Counters = std::unique_ptr<Counter[]>;

void tickCounters(Counters& counters, uint32_t calls = 1)
{
    if (calls > m) return;

    for (auto i{n/10}; i > 0; i--)
    {
        uint32_t index{rand() % n};
        std::cout <<index << std::endl;
        counters[index].add(calls);
    }

    tickCounters(counters, calls + 1);
}

int main()
{
    Counters counters{new Counter[n]};
    tickCounters(counters);
    for (auto i{0u}; i < n; i++)
    {
        std::cout << "Counter" << i << ": " << counters[i].get() << "\n";
    }
    return 0;
}