import requests

url = "https://firestore.googleapis.com/v1/projects/flashixy/databases/(default)/documents/plays/plays"

data = requests.get(url).json()

print(data["fields"]["Chambers"]["integerValue"])
