function getLength(str){
	return str.replace(/[^\x00-\xff]/g,"xx").length;
}

window.onload=function(){

	var oName=document.getElementById('uname');

	var pwd=document.getElementById('passwd');
	var pwd2=document.getElementById('passwd_confirm');

	var name_msg = document.getElementById('name_msg');

	var pwd_msg = document.getElementById('passwd_msg');
	var pwd2_msg = document.getElementById('passwd_confirm_msg');

	var oCount=document.getElementById('count');
	var name_length = 0;
	
	
	oName.onfocus=function(){
		name_msg.style.color="#3c3c3c";
		name_msg.style.dispaly="block";
		name_msg.innerHTML='<i class="ati"></i> 4-20个字符，一个汉字为两个字符';
	}

	oName.onkeyup=function(){
		oCount.style.visibility="visible";
		name_length=getLength(this.value);
		oCount.innerHTML=name_length+"个字符";
	}

	oName.onblur=function(){
		// 含有非法字符，不能为空，长度超过25个字符
	    var re=/[^\w\u4e00-\u9fa5]/g;
		if (re.test(this.value)){
			name_msg.style.color="red";
			name_msg.innerHTML="含有非法字符！";
		}
		else if (this.value==""){
			name_msg.style.color="red";
			name_msg.innerHTML="不能为空！";
		}
		else if (name_length > 20){
			name_msg.style.color="red";
			name_msg.innerHTML="长度不能超过20个字符！";
		}
		else if (name_length < 4){
			name_msg.style.color="red";
			name_msg.innerHTML="长度不能少于4个字符！";
		}
		else{
			name_msg.style.color="green";
			name_msg.innerHTML="Accepted！";
		}
	}
	//密码
	pwd.onfocus=function(){
		pwd_msg.style.display="block";
		pwd_msg.innerHTML="4-20 个字符，只能包含数字或英文字母！";
	}		
	pwd.onkeyup=function(){
		//大于4个字符
		if (this.value.length > 3){
			pwd2.removeAttribute("disabled");
			pwd2_msg.innerHTML="请再输入一次！";
			pwd2_msg.style.display="block";		
		}
		else{
			pwd2.setAttribute("disabled","disabled");
			pwd2_msg.style.display="none";		
		}
	}
	pwd.onblur=function(){
		//不能为空
		if (this.value==""){
			pwd_msg.style.color="red";
			pwd_msg.innerHTML="不能为空！";
		}
		//长度在4-20之间
		else if (this.value.length > 20){
			pwd_msg.style.color="red";
			pwd_msg.innerHTML="长度不能超过20个字符！";
		}
		else if (this.value.length < 4){
			pwd_msg.style.color="red";
			pwd_msg.innerHTML="长度不能少于4个字符！";
		}
		else{
			pwd_msg.style.color="green";
			pwd_msg.innerHTML="Accepted！";
		}
		//正则表达式，只能包含英文字母或数字

	}
	//确认密码		
	pwd2.onblur=function(){
		if (this.value != pwd.value){
			pwd2_msg.style.color="red";
			pwd2_msg.innerHTML="两次输入的密码不一致！";
		}	
		else{
			pwd2_msg.style.color="green";
			pwd2_msg.innerHTML="Accepted！";
		}
	}	
}
