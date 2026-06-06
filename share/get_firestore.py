import requests
import sys

a=sys.argv[1]
f="https://firestore.googleapis.com/v1/projects/flashixy/databases/(default)/documents/plays/"
data = requests.get(f+a).json()
b=data["fields"]["Chambers"]["integerValue"]

if a=="stars":
	data = requests.get(f+a+"_sum").json()
	c=data["fields"]["Chambers"]["integerValue"]
	print(int(c)/int(b)) #/ always performs floating-point division in Python 3, use // for integer division
else:
	print(b)
