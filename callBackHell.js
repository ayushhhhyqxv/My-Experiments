function MakeHTTPSrequest(method,url,callback) {
    const xhr = new XMLHttpRequest()

    xhr.responseType ='json'
    xhr.addEventListener('load',()=>{
        callback(xhr.response)

    })

    xhr.open(method,url)
    xhr.send()
}

MakeHTTPSrequest('GET','https://dummyjson.com/users',(userdata)=>{
    MakeHTTPSrequest('GET',`https://dummyjson.com/posts/user/${userdata.users[0].id}`,(postdata)=>{
         MakeHTTPSrequest('GET',`https://dummyjson.com/comments/post/${postdata.posts[0].id}`,(commentdata)=>{
            // console.log(commentdata.comments[0].id)
            MakeHTTPSrequest('GET',`https://dummyjson.com/users/${commentdata.comments[0].id}`,(fetchuserByid)=>{
                console.log(fetchuserByid)
            })
        })
    })

})
