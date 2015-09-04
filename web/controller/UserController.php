<?php
	class UserController extends Controller {

		function checkSqlError($con) {
			if ($con === FALSE) {
				$this->printfErrorLog(mysql_error());
				return;
			}
		}

		function login() {
			if(isset($_SESSION['username'])) {
				$this->jump(U('Topic','showTopicList'));
			}
			else {
				$UserModel = M('User');
				$UserInfo = $UserModel->getUserByUsername($_POST['username']);
				if(empty($UserInfo) || $UserInfo['password'] != $_POST['password']) {
					$this->error('password is wrong');
				}

				// init userinfo
				$_SESSION['username'] = $UserInfo['username'];
				if (empty($UserInfo['nickname'])){
					$_SESSION['nickname'] = $UserInfo['username'];
				} else {
					$_SESSION['nickname'] = $UserInfo['nickname'];
				}

				$_SESSION['user_id'] = $UserInfo['user_id'];
				$_SESSION['pass_topics'] = $UserInfo['pass_topics'];
				$_SESSION['user_type'] = $UserInfo['user_type'];

				if (NULL == $UserInfo['head_photo']){
					$_SESSION['head_photo'] = 'public/headphoto/default.jpg';
				}
				else{
					$_SESSION['head_photo'] = $UserInfo['head_photo'];
				}
				
				$this->jump(U('Topic','showTopicList'));
			}
		}

		function editUserInfo(){
			$this->checkLogin();

			$UserModel = M('User');
			$UserInfo = $UserModel->getUserByUsername($_SESSION['username']);
			if (empty($UserInfo)){
				$this->error('get userinfo failed',U('Topic','showTopicList'));
			}	
			else{
				$this->assign('userinfo',$UserInfo);
				$this->display('updateuserpage');
			}
		}
		
		function updateUserInfo(){
			$this->checkLogin();

			$userModel = M('User');

			$userName = $_SESSION['username'];			
			$uid = $_SESSION['user_id'];
			$user = $userModel->getPassWdByUsername($userName);

			if (!empty($user['password']) && $user['password'] == $_POST['oldpassword']){
				$userInfo = $_POST;			
				// update password
				if (!empty($userInfo['password'])){
					if ($userInfo['password'] != $userInfo['password_confirm']){
					   $this->error('update userinfo failed, new password is not consisted',U('Topic','showTopicList'));
					}
				}
				else{
					$userInfo['password'] = $userInfo['oldpassword'];
				}
				// update head_photo	
				
				$headphoto = $_FILES['head_photo'];
				if (!empty($headphoto['tmp_name'])){
					if (is_uploaded_file($headphoto['tmp_name'])){
						$suffix = stristr($headphoto['name'],'.');
						$path = "public/headphoto/".$userName.strtotime("now").$suffix;
						if (move_uploaded_file($headphoto['tmp_name'],$path)){
							$userInfo['head_photo'] = $path;				
							$oldpath = $_SESSION['head_photo'];
							if ( "public/head_photo/default.jpg" != $oldpath ){
								@unlink($oldpath);
							}	
							$_SESSION['head_photo'] = $path;
						
						}
						else{
							$this->error('upload photo failed',U('Topic','showTopicList'));
						}
					}
					else{
							$this->error('upload photo failed',U('Topic','showTopicList'));
					}
				}

				unset($userInfo['oldpassword']);
				unset($userInfo['password_confirm']);
				if ( $userModel->updateUserInfo($uid, $userInfo) ){
					$this->success('update info success!');	
				}	
				else{
					$this->error('update info failed!');
				}
			}			
			else{
				$this->error('user is not existed or password is wrong');	
			}
		
			
			print_r($_FILES);

		}
		function logout() {
			session_destroy();
			$this->display('loginpage');
		}

		function assignUserInfo() {
			$this->assign('register_error', 'username is exist');	
			$this->assign('username', $_REQUEST['username']);
			$this->assign('motto', $_REQUEST['motto']);
			$this->assign('sex', $_REQUEST['sex']);
			$this->assign('email', $_REQUEST['email']);
			$this->assign('nickname', $_REQUEST['nickname']);
			$this->assign('realname', $_REQUEST['realname']);
			$this->assign('age', $_REQUEST['age']);
			$this->assign('school', $_REQUEST['school']);	
			$this->assign('class', $_REQUEST['class']);
			$this->assign('qq', $_REQUEST['qq']);
			$this->assign('major', $_REQUEST['major']);
			$this->display('register');	
		}

		function register() {

			if (isset($_SESSION['username'])) {
				$this->error('you need logout first',U('User','login'));
			}
			$userModel = M('User');
			$arr = $userModel->getUserByUsername($_POST['username']);
			if (!empty($arr)) {
				$this->error('用户已存在');
			}

			$con['username'] = $_POST['username'];
			if (strlen($con['username'])<4){
				$this->error('register filed, username is too short!');
			}
			$con['password'] = $_POST['password'];
			if ($con['password'] != $_POST['password2']){
				$this->error('register filed, password is not consistent!');
			}
			$con['at_topics'] = '1';
			$con['at_ladder'] = '1';
			$con['head_photo'] = 'public/headphoto/default.jpg';
			$tTime = getTime();
			$con['register_time'] = $tTime['date']; 

			if ( $userModel->addUser($con) ){
				$this->success('注册成功！');
			}
			else{
				$this->error('register failed for some reasons -o-!');
			}
			
		} 
		
		function showUserInfo(){
			$this->checkLogin();
						
			$userModel = M('User');
			$uid = $_SESSION['user_id'];
			$userInfo = $userModel->getUserInfo($uid);

			$codeModel = M('Code');
			$topicSolvedList = array();
			$topicUnsolvedList = array();
			$topicCodes = $codeModel->getTopicSolvedCodes($uid);
			$solvedList = array();
			foreach ($topicCodes as $code){
				$solvedList[$code['problem_id']] = true;
			}
			
			$passTopics = explode(',',$userInfo['pass_topics']);
			$atTopics = explode(',',$userInfo['at_topics']);
			$topicList = array_merge($passTopics,$atTopics);
			$topicModel = M('Topic');
			foreach ($topicList as $topicid){
				$topicInfo = $topicModel->getTopicInfo($topicid);
				if (!empty($topicInfo['problem_ids'])){
					$topicProblems = explode(',',$topicInfo['problem_ids']);
					foreach ($topicProblems as $id => $problemId){
						if (isset($solvedList[$problemId])){
							$topicSolvedList[$topicid] []= $id + 1;
						} else {
							$topicUnsolvedList[$topicid] []= $id + 1;
						}
					}
				}
			}
			
			$this->assign('topicsolved', $topicSolvedList);
			$this->assign('topicunsolved', $topicUnsolvedList);
			$this->assign('userinfo', $userInfo);
			$this->display('userinfo');
		}


		function getUserInfo() {
			if (!isset($_SESSION['username'])) {
				$this->display('loginpage');
				return;
			}
			$uid = $_GET['uid'];
			$userModel = M('User');
			$userInfo = $userModel->getUserInfo($uid);
			checkSqlError($userInfo);
			if (!empty($userInfo)) {
				$this->assign('userInfo', $userInfo);
			}
			else echo 'no such User';
		}
	    	
	}	

?>
