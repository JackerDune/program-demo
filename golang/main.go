package main

import (
    "fmt"
    "encoding/json"
)

func main() {
    fmt.Println("Hello world!")
    test_json := []byte(`{
        "code": 0,
        "data": [
            {"sn":"111111111", "value":200},
            {"sn":"222222222", "value":300}
        ]
    }`)
   
    jdata := make(map[string]interface{})
    err := json.Unmarshal(test_json, &jdata)
    if (err == nil) {
        fmt.Println(jdata)
    } else {
        fmt.Println(err) 
    }
    fmt.Println(jdata["code"])
    fmt.Println(jdata["data"])
    fmt.Println("sn is: ",
        jdata["data"].([]interface{})[0].(map[string]interface{})["sn"])
    fmt.Println("value is: ",
        jdata["data"].([]interface{})[0].(map[string]interface{})["value"])
}
