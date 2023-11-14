#include <iostream>
#include <climits>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;

extern "C" int FindMaxAsm(const int* array, int length);

int FindMax(vector<int>& nums) {
    if (nums.empty()) {
        cerr << "Error: Vector is empty." << endl;
        return INT_MIN;
    }

    int max = nums[0];
    for (int num : nums) {
        if (num > max) {
            max = num;
        }
    }

    return max;
}

__declspec(naked) int FindMaxAsm(const int* array, int length) {
    __asm {
        push ebp
        mov ebp, esp
        push edi
        push esi
        push ebx

        mov esi, [ebp + 8]
        mov ecx, [ebp + 12]
        mov ebx, INT_MIN

        cmp ecx, 0; Vérifier si la longueur est nulle
        jle end_boucle; Si c'est le cas, sauter à la fin

        start_loop :
        mov eax, [esi]
            cmp eax, ebx
            jle skip
            mov ebx, eax

            skip :
        add esi, 4
            dec ecx; Décrémenter le compteur de boucle
            jnz start_loop; Sauter à start_loop tant que le compteur n'est pas nul

            end_boucle:
        pop ebx
            pop esi
            pop edi
            pop ebp
            ret
    };
}

// Fonction pour générer un tableau aléatoire
void generateRandomArray(vector<int>& nums, int size) {
    nums.clear();
    nums.reserve(size);
    srand(static_cast<unsigned>(time(nullptr)));  // Initialiser la graine pour rand()

    for (int i = 0; i < size; ++i) {
        nums.push_back(rand());
    }
}

template <typename Function>
double measureExecutionTime(Function func, const int* array, int length) {
    auto start = chrono::high_resolution_clock::now();
    func(array, length);
    auto end = chrono::high_resolution_clock::now();

    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}

int main() {
    const int arraySize = 1000;
    vector<int> nums;

    generateRandomArray(nums, arraySize);

    // Mesurer le temps d'exécution pour la fonction en C++
    double cppExecutionTime = measureExecutionTime(FindMax, nums.data(), nums.size());

    // Mesurer le temps d'exécution pour la fonction en assembleur
    double asmExecutionTime = measureExecutionTime(FindMaxAsm, nums.data(), nums.size());

    cout << "Temps d'execution (C++): " << cppExecutionTime << " microsecondes" << endl;
    cout << "Temps d'execution (Asm): " << asmExecutionTime << " microsecondes" << endl;

    // Calculer le rapport entre le temps d'exécution en assembleur et en C++
    double ratio = asmExecutionTime / cppExecutionTime;

    cout << "Rapport de temps d'execution (Asm / C++): " << ratio << endl;

    return 0;
}