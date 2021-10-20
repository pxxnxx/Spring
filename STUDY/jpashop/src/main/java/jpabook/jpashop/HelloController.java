package jpabook.jpashop;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;

@Controller
public class HelloController {

    // /hello 매핑
    @GetMapping("hello")
    public String hello(Model model) {
        model.addAttribute("data","hello!!!");
        return "hello"; // 화면 이름
    }
}
