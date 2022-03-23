import pandas as pd
import pingouin as pg

# reading the file
data = pd.read_csv("wilcoxon/NR30vsHS30.csv")

# wilcoxon statistical analysis
data.columns = ["NR30", "HS30"]

wilcoxon1 = pg.wilcoxon(data["NR30"], data["HS30"], alternative='two-sided')
print(wilcoxon1)

# pairwise correlation
# pairwise = data.corr(method='pearson')
# print(pairwise)
