
window.onload=function(){

	var oPid=document.getElementById('pid');
	var oName=document.getElementById('pname');

	$('#pid').blur(function(){
		$.get(
	    	'admin.php?c=Topic&a=getProblemName',
			{
				pid:$("#pid").val()		
			},
			function (jsonArr) {
				var obj=eval("("+jsonArr+")");
				if ( null == obj.pname ){
					oName.style.color="red";
					oName.value="该题目不存在！";	
				}
				else{
					oName.style.color="#123456";
					oName.value=obj.pname;
				}
			}
		);
	});

}
