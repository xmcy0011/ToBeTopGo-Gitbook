package main

import (
	"encoding/json"
	"log"

	"github.com/tidwall/gjson"
)

type UserRequest struct {
	UserName string `json:"userName"`
	NickName string `json:"nick_name"`
}

func main() {
	jsonStr := `{"userName": "admin", "nick_name": "管理员", "info":{ "age":18 }}`

	// 方式一：序列化成map（不常用）
	anyMap := make(map[string]interface{}, 0)
	if err := json.Unmarshal([]byte(jsonStr), &anyMap); err != nil {
		panic(err)
	}
	log.Println("Unmarshal to map result:", anyMap)

	// 方式二：序列化成对象，经常使用
	req := UserRequest{}
	if err := json.Unmarshal([]byte(jsonStr), &req); err != nil {
		panic(err)
	}
	log.Println("Unmarshal to struct:", req)

	// 方式三：不反序列化，只读取单个key，经常使用。适合特别复杂的json字符串，或者有多种if else结构的场景
	userName := gjson.Get(jsonStr, "userName")
	nickName := gjson.Get(jsonStr, "nick_name")
	age := gjson.Get(jsonStr, "info.age").Int()
	log.Println("get raw value by key:", userName, nickName, age)
}
