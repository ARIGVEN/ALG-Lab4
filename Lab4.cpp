#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <map>
#include <iomanip>

int dice(const std::string& specStr) {
    static std::mt19937 gen(std::random_device{}());
    int total = 0;
    size_t start = 0;
    
    while (start < specStr.length()) {
        size_t comma = specStr.find(',', start);
        std::string dice = (comma == std::string::npos) ? 
                           specStr.substr(start) : 
                           specStr.substr(start, comma - start);
        
   
        size_t d_pos = dice.find('d');
        if (d_pos == std::string::npos) continue;
        
        int count = 1;
        if (d_pos > 0) {
            count = std::stoi(dice.substr(0, d_pos));
        }
        
        
        int sides = 0, modifier = 0;
        size_t plus = dice.find('+', d_pos);
        size_t minus = dice.find('-', d_pos);
        
        if (plus != std::string::npos) {
            sides = std::stoi(dice.substr(d_pos + 1, plus - d_pos - 1));
            modifier = std::stoi(dice.substr(plus + 1));
        } else if (minus != std::string::npos) {
            sides = std::stoi(dice.substr(d_pos + 1, minus - d_pos - 1));
            modifier = -std::stoi(dice.substr(minus + 1));
        } else {
            sides = std::stoi(dice.substr(d_pos + 1));
        }
        
        
        std::uniform_int_distribution<int> dice_drop(1, sides);
        for (int i = 0; i < count; ++i) {
            total += dice_drop(gen);
        }
        total += modifier;
        
        start = (comma == std::string::npos) ? specStr.length() : comma + 1;
    }
    
    return total;
}

int main() {
    // Пример использования
    std::cout << "Пример броска: \"2d6,d10+3\" = " 
              << dice("2d6,d10+3") << "\n\n";
    
    // Исследуем распределения
    std::vector<std::string> configs = {"1d6", "2d6", "3d6", "1d10", "2d10", "3d10"};
    const int trials = 100000;
    
    for (const auto& config : configs) {
        std::map<int, int> freq;
        
        // Моделируем броски
        for (int i = 0; i < trials; ++i) {
            int result = dice(config);
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