#include <iostream>
#include <vector>
#include <climits>
#include <bitset>

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

void PrintBinary(int num) {
    std::bitset<32> binary(num);
    std::cout << "Complement a deux de " << num << " : " << binary << std::endl;
}

int main() {
    std::vector<int> nums = { -3, 5, -1, 10, -15, 100, 7, 34 };  // Notre tableau

    int maxCpp = FindMax(nums);
    int maxAsm = FindMaxAsm(nums.data(), nums.size());

    std::cout << "Le plus grand nombre avec la fonction ecrite en C++ est : " << maxCpp << std::endl;
    std::cout << "Le plus grand nombre avec la fontion ecrite en Assembleur est : " << maxAsm << std::endl;

    // Parcourir le tableau et afficher les nombres négatifs en complément à deux
    for (int num : nums) {
        if (num < 0) {
            PrintBinary(num);
        }
    }

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

