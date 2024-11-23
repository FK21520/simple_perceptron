#include <math.h>

#define EPOCHS 140  // 学習回数
#define Eta 0.2 

void SimplePerceptron(Data *data, int feature, int line, float *x1, float *x2, State *state)
{
    float b; // バイアス
    float MSE = 0.0, RMSE = 1.0;
    float *w = (float*)malloc(feature * sizeof(float));  // 重み
   
    //重みとバイアスの初期化
    srand((unsigned int)time(NULL));
    for (int i = 0; i < feature; i++) 
    {
        w[i] = (rand() % 100) / 10.0;  // 適当な初期値
    }
    b = (rand() % 100) / 10.0;  // 適当な初期バイアス
    
    // 学習ループ
    for (int epoc = 0; epoc < EPOCHS; epoc++) 
    {
        // printf("%d\n", epoc);
        for (int i = 0; i < line; i++)
        {
            // 出力の計算
            float output = w[0] * data->x[i] + w[1] * data->y[i] + b;
            int predicted_label = (output >= 0) ? 1 : -1;
            int error = data->label[i] - predicted_label;
            
            // 誤差があれば重みとバイアスを更新
            if (predicted_label != data->label[i]) 
            {
                w[0] += Eta * data->x[i] * error;  // 重みの更新
                w[1] += Eta * data->y[i] * error;  // 重みの更新
                b += Eta * error;  // バイアスの更新
            }
            // 誤差の二乗を累計
            MSE += error * error;
        }
        
        // 誤差が一定以下なら終了
        RMSE = sqrt(MSE / line);
    
        if (RMSE < 0.001) 
        {
            printf("学習が完了しました。エポック: %d\n", epoc);
            break;
        }
        MSE = 0; // MSEの初期化
    }

    // 分離線の係数を返す（y = -(w[0]/w[1]) * x - b/w[1]）
    state->avx = state->sumx / (float)line;
    state->avy = state->sumy / (float)line;

    //元データ-データの平均で計算しているためデータをスケールする
    //x'=x-avx, y'=y-avyとなるのでy'=mx'+c(mは切片,cは傾き)とするとy'=m(x-avx)+cとなり,y'=mx-mavx+c (1)
    //y=y'+avyなので(1)を代入するとy=mx--mavx+c+avyとなるのでmとcに元の傾きと切片を代入すると以下の式になる
    *x1 = -(w[0] / w[1]);  // 傾き
    // *x2 = -(b / w[1]);     // 切片
    *x2 = (w[0]/w[1])*state->avx - (b/w[1]) + state->avy;

    free(w);  // メモリ解放
}