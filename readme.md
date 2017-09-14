# Paiza 怪盗paizaからの挑戦状

<https://paiza.jp/poh/phantom_thief>

## メモ

-   greedy: 144850m
-   初期解greedy + 頂点swap: 141672m
-   初期解greedy + 2opt: 137515m
-   初期解greedy + 2opt + SA: 131167m
-   初期解greedy + 2opt + 3opt + SA: 130525m
-   初期解MST + 2opt + 3opt + SA: 130371m

-   総提出回数: 52回
-   最終結果: 5位

## テストケース

-   Test case 1: 152m
-   Test case 2: 401m
-   Test case 3: 22479m
-   Test case 4: 31045m
-   Test case 5: 76291m

## 感想等

-   TSPやるだけじゃんと思ったが「有名なやつじゃないと競プロガチ勢が上位独占しちゃうよね」みたいなこと言ってる人がいてなるほど
-   厳密解でちゃうのはだめなのでは
-   130371.0mが厳密解でなかった、小数点以下がちゃんと表示されてなくて実は、という展開 -> なかった
-   ケースが実質ひとつなのでガチャ
-   curlは便利
-   賞金枠埋まった時点でみんな「はい解散」みたいな雰囲気だしてたの好き
-   Lin-Kernighan法ってのもあるらしい
-   まじめにやるつもりでrepo切ったのにあまり必要でなかった
