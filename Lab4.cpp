#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <map>
#include <iomanip>

class DiceRoller {
private:
    std::mt19937 gen;
    
public:
    DiceRoller() : gen(std::random_device{}()) {}
    
    int dice(const std::string& specStr) {
        int total = 0;
        size_t start = 0;
        
        while (start < specStr.length()) {
            size_t comma = specStr.find(',', start);
            std::string token = (comma == std::string::npos) ? 
                               specStr.substr(start) : 
                               specStr.substr(start, comma - start);
            
            // Парсим количество дайсов
            size_t d_pos = token.find('d');
            if (d_pos == std::string::npos) continue;
            
            int count = 1;
            if (d_pos > 0) {
                count = std::stoi(token.substr(0, d_pos));
            }
            
            // Парсим количество сторон и модификатор
            int sides = 0, modifier = 0;
            size_t plus = token.find('+', d_pos);
            size_t minus = token.find('-', d_pos);
            
            if (plus != std::string::npos) {
                sides = std::stoi(token.substr(d_pos + 1, plus - d_pos - 1));
                modifier = std::stoi(token.substr(plus + 1));
            } else if (minus != std::string::npos) {
                sides = std::stoi(token.substr(d_pos + 1, minus - d_pos - 1));
                modifier = -std::stoi(token.substr(minus + 1));
            } else {
                sides = std::stoi(token.substr(d_pos + 1));
            }
            
            // Бросаем кости
            std::uniform_int_distribution<int> dist(1, sides);
            for (int i = 0; i < count; ++i) {
                total += dist(gen);
            }
            total += modifier;
            
            start = (comma == std::string::npos) ? specStr.length() : comma + 1;
        }
        
        return total;
    }
};

int main() {
    DiceRoller roller;
    
    // Пример использования
    std::cout << "Пример броска: \"2d6+2,3d10\" = " 
              << roller.dice("2d6+2,3d10") << "\n\n";
    
    // Исследуем распределения
    std::vector<std::string> configs = {"1d6", "2d6", "3d6", "1d10", "2d10", "3d10"};
    const int trials = 100000;
    
    for (const auto& config : configs) {
        std::map<int, int> freq;
        
        // Моделируем броски
        for (int i = 0; i < trials; ++i) {
            int result = roller.dice(config);
            freq[result]++;
        }
        
        // Находим максимум для масштабирования гистограммы
        int maxFreq = 0;
        for (const auto& [val, count] : freq) {
            if (count > maxFreq) maxFreq = count;
        }
        
        // Выводим распределение
        std::cout << config << " (" << trials << " бросков):\n";
        std::cout << "Знач Частота Вероятность Гистограмма\n";
        
        for (const auto& [val, count] : freq) {
            double prob = (double)count / trials * 100;
            int barLength = (int)((double)count / maxFreq * 40);
            
            std::cout << std::setw(4) << val << " "
                     << std::setw(7) << count << " "
                     << std::setw(6) << std::fixed << std::setprecision(1) << prob << "% ";
            
            for (int i = 0; i < barLength; ++i) {
                std::cout << "0";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
    
    return 0;
}