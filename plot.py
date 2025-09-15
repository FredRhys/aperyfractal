import pandas as pd
import matplotlib.pyplot as plt
import io
from PIL import Image

url = 'output.csv'
df = pd.read_csv(url)

df.plot.scatter('x',
                'y',
                grid=False,
                s=0.1,
				figsize=(10,10),
               )
plt.savefig('output.png')
plt.show()