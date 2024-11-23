typedef struct {
    float *x;
    float *y;
    int *label;
} Data;

typedef struct 
{
    float sumx, sumy; //xとyのそれぞれの合計値
    float avx, avy; //xとyのそれぞれの平均
    float minx, miny; //xとyのそれぞれの最小値
    float maxx, maxy; //xとyのそれぞれの最大値
} State;

void InitializeData(Data *data, int *line, State *state) 
{
    data->x = (float*)malloc(*line * sizeof(float));
    data->y = (float*)malloc(*line * sizeof(float));
    data->label = (int*)malloc(*line * sizeof(int));

    if (data->x == NULL || data->y == NULL || data->label == NULL) 
    {
        printf("メモリの割り当てに失敗しました。\n");
        exit(1);
    }

    state->sumx, state->sumy = 0.0;
}

void FreeData(Data *data) 
{
    free(data->x);
    free(data->y);
    free(data->label);
}

// 行数をカウントする関数
int CountLinesInFile(FILE **fp, const char *filename) 
{
    int lines = 0;

     char buf[BUFSIZ];

    if ((*fp = fopen(filename, "r")) == NULL)
    {
        printf("ファイルが開けません1\n");
        exit(1);
    }

    while (fgets(buf, BUFSIZ, *fp) != NULL)
    {
        lines += 1; // line++とかくとバグる
    }

    rewind(*fp);

    return lines-1;
}

float Min(float data, float min)
{
    if(data < min)
    {
        min = data;
    }else{
        min = min;
    }
    return min;
}

float Max(float data, float max)
{
    if(data > max)
    {
        max = data;
    }else{
        max = max;
    }
    return max;
}


void ReadCsv(FILE *fp_csv, int *line, Data *data, State *state)
{   
    float xc, yc;
    int labelc;
    
    // ヘッダーを読み飛ばす
    fscanf(fp_csv, "%*[^,],%*[^,],%*[^,\n]\n");

    // データを1行ずつ読み込む
    for(int i = 0; i < *line; i++)
    {
        fscanf(fp_csv, "%f,%f,%d", &xc, &yc, &labelc);
        data->x[i] = xc;
        data->y[i] = yc;
        data->label[i] = labelc;
        state->sumx = state->sumx + xc; 
        state->sumy = state->sumy + yc;
        if(i == 0)
        {
            state->minx = xc, state->maxx = xc;
            state->miny = yc, state->maxy = yc;
        }
        state->minx = Min(xc, state->minx), state->maxx = Max(xc, state->maxx);
        state->miny = Min(yc, state->miny), state->maxy = Max(yc, state->maxy);
    }
    fclose(fp_csv);
}

Data global_data;  // グローバルデータ

// 比較関数
int compare(const void *a, const void *b) 
{
    int idx_a = *(int *)a;
    int idx_b = *(int *)b;
    return global_data.label[idx_a] - global_data.label[idx_b];
}

// ラベルに基づいてソートする関数
void SortByLabel(Data *data, int line) 
{
    // グローバルデータに構造体を代入
    global_data = *data;

    // インデックスを保持する配列を作成
    int *indices = (int *)malloc(line * sizeof(int));
    for (int i = 0; i < line; i++) 
    {
        indices[i] = i;
    }

    // インデックスを基準にqsortでソート
    qsort(indices, line, sizeof(int), compare);

    // ソートされたデータを格納するための一時配列を作成
    float *sorted_x = (float *)malloc(line * sizeof(float));
    float *sorted_y = (float *)malloc(line * sizeof(float));
    int *sorted_label = (int *)malloc(line * sizeof(int));

    // ソートされたインデックスに従ってデータをコピー
    for (int i = 0; i < line; i++) 
    {
        sorted_x[i] = data->x[indices[i]];
        sorted_y[i] = data->y[indices[i]];
        sorted_label[i] = data->label[indices[i]];
    }

    // 元のデータにソート結果を反映
    for (int i = 0; i < line; i++) 
    {
        data->x[i] = sorted_x[i];
        data->y[i] = sorted_y[i];
        data->label[i] = sorted_label[i];
    }

    // メモリ解放
    free(indices);
    free(sorted_x);
    free(sorted_y);
    free(sorted_label);
}

//正規化
void Normalize(Data *data, int line, State *state)
{
    for(int i = 0; i < line; i++)
    {
        data->x[i] = (data->x[i] - state->minx) / (state->maxx - state->minx);
        data->y[i] = (data->y[i] - state->miny) / (state->maxy - state->miny);
    }
}

//データ変形
void Trans(Data *data, int line, State *state)
{
    state->avx = state->sumx / (float)line;
    state->avy = state->sumy / (float)line;

    // data-平均 
    for(int i = 0; i < line; i++)
    {
        data->x[i] = data->x[i] - state->avx;
        data->y[i] = data->y[i] - state->avy;
    }
}

