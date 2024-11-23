void plot(const char *filename, float x1, float x2)
{
    FILE *gp;
    if ((gp = popen("gnuplot -persist", "w")) == NULL) 
    {
		printf("ファイルが見つかりません");
		exit(1);
	}

    fprintf(gp, "set datafile separator ','\n");          // カンマ区切りのデータを使う
    fprintf(gp, "set title 'Perceptron Classification'\n");  // タイトル設定
    fprintf(gp, "set xlabel 'Height (cm)'\n");            // X軸ラベル
    fprintf(gp, "set ylabel 'Weight (kg)'\n");           // Y軸ラベル

    // セミコロンを追加して、2つのスタイル設定を正しく区切る
    fprintf(gp, "set style line 1 lc rgb 'red' pt 7;\n");  // ラベル1は赤
    fprintf(gp, "set style line 2 lc rgb 'blue' pt 7;\n"); // ラベル-1は青

    // データをプロットする
    fprintf(gp, "plot '%s' using 1:2:(($3==1)?1:2) with points pointtype 7 palette\n", filename);
    
    //境界線のプロット
    fprintf(gp, "replot %f * x + %f title 'Decision Boundary' with lines lc rgb 'green'\n", x1, x2);

    // プロットを表示
    fflush(gp);
    fprintf(gp, "pause -1\n");  // ユーザーの操作まで表示を保持
    pclose(gp);
}

