<?php
    class CodeController extends Controller {
        function submit() {
            if(!isset($_SESSION['username'])) {
                $this->display('loginpage');
            }
            else {
                $pid = $_GET['pid'];
                $this->assign('pid', $pid);
                $this->assign('username', $_SESSION['username']);
                $this->display('submit');
            }
        }
        function submitCode() {

        }
        function status() {
            if(!isset($_SESSION['username'])) {
                $this->display('loginpage');
            }
            else {
                $pid = $_GET['pid'];
                $uid = $_SESSION['uid'];
                $username = $_SESSION['username'];
                $codeModel = M('Code');
                $result = $codeModel->getByUid($uid);
                var_dump($result);
                $result = $this->getPage($pid, $result);
                $this->assign('uid', $uid);
                $this->assign('username', $username);
                $this->assign('hasPre', $result['hasPre']);
                $this->assign('hasNext', $result['hasNext']);
                $this->assign('pid', $result['pid']);
                $this->assign('rows', $result['rows']);
                $this->display('status');
            }
        }
		function showRejudgePage(){
			$this->checkAdmin();
			$this->display('rejudge');
		}
		function rejudge(){
			$this->checkAdmin();
			$rejudgeType = $_POST['judgetype'];
			$codeModel = M('Code');
			if ($rejudgeType == 1){
				$status = $_POST['judge_status'];
				$conditions = 'judge_status='.$status;
				$newstatus = 0;
				if ($codeModel->updateCodeStatus($conditions, $newstatus)){
					$this->success('rejudge success', U('Code','showRejudgePage'));
				} else {
					$this->error('rejudge error', U('Code','showRejudgePage'));
				}
			} else if ($rejudgeType == 2) {
				$code_id = $_POST['code_id'];	
				$conditions = 'code_id='.$code_id;
				$newstatus = 0;
				if ($codeModel->updateCodeStatus($conditions, $newstatus)){
					$this->success('rejudge success', U('Code','showRejudgePage'));
				} else {
					$this->error('rejudge error', U('Code','showRejudgePage'));
				}
			} else if ($rejudgeType == 3){
				$problem_id = $_POST['problem_id'];
				$conditions = 'problem_id='.$problem_id;
				$newstatus = 0;
				if ($codeModel->updateCodeStatus($conditions, $newstatus)){
					$this->success('rejudge success', U('Code','showRejudgePage'));
				} else {
					$this->error('rejudge error', U('Code','showRejudgePage'));
				}
			}
		}
    }
?>
