<?php
    class IndexController extends Controller {
        public function index() {
            if(isset($_SESSION['username'])) {
				$this->jump(U('Topic','showTopicList'));
            }
            else {
                $this->display('loginpage');
            }
        }
    }
?>
