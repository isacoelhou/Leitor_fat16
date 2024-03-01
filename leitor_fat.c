#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void boot_recorder (){}

int main()
{

    FILE *fp;
    short int bytes_per_sector, directory_entries, reserved_sectors , sector_per_fat;
    unsigned char sector_per_cluster, number_of_fat;

    fp = fopen("./fat16_1sectorpercluster.img", "rb");

    fseek(fp, 11, SEEK_SET);
    fread(&bytes_per_sector, 2,1, fp);

    fseek(fp, 13, SEEK_SET);
    fread(&sector_per_cluster, 1,1,fp);

    fseek(fp, 22, SEEK_SET);
    fread(&sector_per_fat, 2,1,fp);

    fseek(fp, 16, SEEK_SET);
    fread(&number_of_fat, 1,1,fp);

    fseek(fp, 17, SEEK_SET);
    fread(&directory_entries,2,1,fp);

    fseek(fp, 14, SEEK_SET);
    fread(&reserved_sectors ,2,1, fp);

    printf("============== DADOS DO BOOT RECORD: ==============\n\n");
    printf("Bytes por sector: %hd \n", bytes_per_sector);
    printf("Número de entradas no diretório raiz: %hd \n", directory_entries);
    printf("Número de setores reservados: %hd \n", reserved_sectors );
    printf("Número de setores por FAT: %hd \n", sector_per_fat);
    printf("Sector per cluster: %x \n", sector_per_cluster);
    printf("Número de FAT: %x \n", number_of_fat);


    int FAT1 = reserved_sectors;
    int FAT2 = FAT1 + sector_per_fat;
    int root_dir = FAT2 + sector_per_fat;
    int data = root_dir + ((directory_entries * 32)/ bytes_per_sector);

    printf("\n\n============== ORGANIZAÇÃO DA FAT: ==============\n\n");

    printf("FAT1 começa no setor %d, endereço %d\n", FAT1 ,FAT1*512);
    printf("FAT2 começa no setor %d, endereço %d\n", FAT2, FAT2*512);
    printf("Root dir começa no setor %d, endereço %d\n", root_dir ,root_dir*512);
    printf("A área de dados começa no setor %d, endereço %d\n\n\n", data, data*512);

    short int first_cluster;
    unsigned char LFN;
    int size;
    char file_name[12];
    int entry_number = 0;
    char tipo[15];

    printf("============== INFORMAÇÕES VÁLIDAS DO ROOT DIR:===============\n");

    while(1){
    fseek(fp, (root_dir*bytes_per_sector)+(entry_number*32), SEEK_SET);
    fread(&file_name, 12,1, fp);
    entry_number++;

    if (file_name[11] == 15)
        continue;
    
    if (file_name[0] == -27)
        continue;
    
    if (file_name[0] == 0)
        break;

    if (file_name[11] == 32)
        strcpy(tipo, "arquivo");
    else 
        strcpy(tipo, "diretório");
 
    fseek(fp,(((root_dir*bytes_per_sector)+((entry_number-1)*32))+26), SEEK_SET);
    fread(&first_cluster, 2,1, fp);

    fseek(fp,(((root_dir*bytes_per_sector)+((entry_number-1)*32))+28), SEEK_SET);
    fread(&size, 4,1, fp); 
    printf("\nNome: %s\n", file_name);
    printf("Primeiro: %hd\n", first_cluster);
    printf("Tamanho: %d\n", size);
    printf("Tipo: %s\n", tipo);

    }

    printf("\n\n============== CONTEÚDO DO ARQUIVO ==================\n\n");


    fclose(fp);

    return 0;
}
