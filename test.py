import requests

url = 'https://api.fpt.ai/hmi/tts/v5'

headers = {
    'api-key': 'G6oV0bbP1jekKSiwkoIKYDVxKN30CGBc',
    'speed': '',
    'voice': 'minhquang'
}

response = requests.request('POST', url, data=f.encode('utf-8'), headers=headers)

print(response.text)
