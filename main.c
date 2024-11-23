#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "tool.h"
#include "simple_perseptron.h"
#include "plot.h"

int main() {
    Data data;
    State state;
    
    int line = 0;
    
    FILE *gp, *fp, *fp2, *fp3;

    char *filename = "perceptron_data.csv";
    float x1, x2 = 0.0;

    line = CountLinesInFile(&fp, filename);
    
    // データの初期化と読み込み
    InitializeData(&data, &line, &state);
    
    // csvファイルの読み込み
    ReadCsv(fp, &line, &data, &state);
    
    // ラベルに基づいてソート
    SortByLabel(&data, line);

    //Normalize(&data, line, &state);
    Trans(&data, line, &state);

    //  単純パーセプトロン
    SimplePerceptron(&data, 2, line, &x1, &x2, &state);
   
    //プロット
    plot(filename, x1, x2);

    // メモリの解放
    FreeData(&data);

    return 0;
}

