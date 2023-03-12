import requests
import json 

def hello():
    print("Hello world!")


def httpget(title, url, timeout=10):
    resp = requests.get(url, timeout=timeout)
    print("==== {} Headers Begin====:".format(title))
    print(resp.headers)
    print("==== {} Headers End====:".format(title))
    print("==== {} Title Begin====:".format(title))
    print(resp.content)
    print("==== {} Title End====:".format(title))


def list_loop():
    t_list = ["ab", "bc", "cd"]
    for i in range(len(t_list)):
        print(t_list[i])
    print(json.dumps(t_list))


def mac_add(mac, det):
    try:
        mac_list = mac.split(":")
        l = len(mac_list)
        if l != 6:
            print("mac input error :{}".format(mac))
            return
        mac_value = (int(mac_list[4], 16) << 8) + int(mac_list[5], 16)
        mac_value = mac_value + det
        mac_list[4] = "{:02x}".format(((mac_value >> 8) & 0xff))
        mac_list[5] = "{:02x}".format(mac_value & 0xff)
        mac = ":"
        print(mac.join(mac_list))
    except Exception as e:
        print("caculate mac error :{}".format(str(e)))


if __name__ == "__main__":
    hello()
    #httpget("Baidu response", "https://www.baidu.com")
    #httpget("163 response", "https://www.163.com")
    #httpget("taobao response", "https://www.taobao.com")
    #httpget("github response", "https://www.github.com")
    list_loop()
    mac_add("00:01:02:03:04:05", 1)
    mac_add("00:01:02:03:04:ff", 4)
    mac_add("00:01:02:03:04:ff", 5)
    mac_add("00:01:02:03:04:", 5)
