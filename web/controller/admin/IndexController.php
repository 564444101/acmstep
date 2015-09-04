<?php
    class IndexController extends Controller {
        public function index() {
			if(isset($_SESSION['username'])) {
				$this->display('admin');
			}
			else {
				$this->display('loginpage');
			}
        }
    }
?>
