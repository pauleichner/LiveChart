import yfinance as yf
import pandas as pd

# Choose a Ticker Symbol
ticker = "AAPL"
# Yfinance API only allows to load the last 5d worth of 1min Candles. If you want to Backtest a different time feel free to Change that...
data = yf.download(ticker, period="5d", interval="1m")



###################################################
######## DONT CHANGE UPON THIS POINT !!!!! ########
###################################################
data.reset_index(inplace=True)
data['Datetime'] = data['Datetime'].dt.strftime('%Y-%m-%d %H:%M')  # Format: YYYY-MM-DD HH:MM
data = data[['Datetime', 'Open', 'High', 'Low', 'Close', 'Volume']]
file_path = "../data/Apple_1MD.csv"
data.to_csv(file_path, sep='\t', index=False, header=False, float_format='%.5f')

print(f"Data saved to {file_path} in the correct format.")
###################################################