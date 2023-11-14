#include <iostream>
#include <vector>
#include <climits>
#include <cstdlib>
#include <chrono>
#include <cstdint> 

extern "C" int FindMaxAsm(const int* array, int length);

int FindMax(const std::vector<int>& nums) {
    int max = INT_MIN;
    for (int num : nums) {
        if (num > max) {
            max = num;
        }
    }
    return max;
}

std::vector<int32_t> generateRandomArray(size_t size) {
    std::vector<int32_t> nums(size);
    for (auto& num : nums) {
        num = rand(); // génère un nombre aléatoire
    }
    return nums;
}

int main() {
    const size_t ARRAY_SIZE = 1000000; // Taille du tableau
    const int NUM_TRIALS = 10; // Nombre d'essais

    srand(time(NULL)); // Initialise le générateur de nombres aléatoires

    double totalCppTime = 0.0;
    double totalAsmTime = 0.0;

    //Verifier si c'est 32 bits
    std::vector<int32_t> nums = generateRandomArray(ARRAY_SIZE);
    std::cout << "Le format est : " << sizeof(nums[0]) << " Octets, Ce qui donne 32 bits\n\n";

    for (int i = 0; i < NUM_TRIALS; ++i) {
        std::vector<int32_t> nums = generateRandomArray(ARRAY_SIZE);

        // Mesure pour la fonction C++
        auto startCpp = std::chrono::high_resolution_clock::now();
        int maxCpp = FindMax(nums);
        auto endCpp = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> cppTime = endCpp - startCpp;
        totalCppTime += cppTime.count();

        // Mesure pour la fonction en assembleur
        auto startAsm = std::chrono::high_resolution_clock::now();
        int maxAsm = FindMaxAsm(nums.data(), nums.size());
        auto endAsm = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> asmTime = endAsm - startAsm;
        totalAsmTime += asmTime.count();

        // Affichage des temps d'exécution
        std::cout << "Essai " << i + 1 << ":\n";
        std::cout << "Temps d execution C++: " << cppTime.count() << " millisecondes\n";
        std::cout << "Temps d execution Assembleur: " << asmTime.count() << " millisecondes\n\n";
    }

    double averageCppTime = totalCppTime / NUM_TRIALS;
    double averageAsmTime = totalAsmTime / NUM_TRIALS;

    // Affichage des temps d'exécution moyens
    std::cout << "Temps d execution moyen C++: " << averageCppTime << " millisecondes\n";
    std::cout << "Temps d execution moyen Assembleur: " << averageAsmTime << " millisecondes\n";

    return 0;
}

__declspec(naked) int FindMaxAsm(const int* array, int length)
{
    __asm {
        push ebp
        mov ebp, esp
        push edi
        push esi
        push ebx

        ; Initialisation des registres
        mov esi, [ebp + 8]; array
        mov ecx, [ebp + 12]; length
        mov eax, INT_MIN; max

        ; Vérifie si le tableau est vide
        test ecx, ecx
        jz end_loop

        ; Boucle pour parcourir le tableau
        start_loop :
        mov ebx, [esi]; Charge lélément actuel du tableau
            cmp eax, ebx; Compare avec le max actuel
            jge skip; Si le max actuel est plus grand, saute
            mov eax, ebx; Met à jour le max
            skip :
        add esi, 4; Avance au prochain entier
            dec ecx; Décrémente le compteur
            jnz start_loop; Continue si non nul

            ; Fin de la boucle, eax contient le max
            end_loop :
            pop ebx
            pop esi
            pop edi
            pop ebp
            ret
    }
}
