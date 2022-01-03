package com.homepageTest.board.controller;

import com.homepageTest.board.domain.entity.Board;
import com.homepageTest.board.dto.BoardDto;
import com.homepageTest.board.service.BoardService;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@Controller
public class BoardController {

    private BoardService boardService;

    public BoardController(BoardService boardService) {
        this.boardService = boardService;
    }

    @GetMapping("/")
    public String list(Model model){
        System.out.println("Board Controller :: list");
//        List<BoardDto> boardDtoList=boardService.getBoardList();
//        model.addAttribute("postList",boardDtoList);
//        System.out.println(boardDtoList);

        List<BoardDto> boardDtoList =boardService.getBoardList();
        model.addAttribute("postList",boardDtoList);
        System.out.println("BoardController :: Home");
        return "board/list.html";
    }

    @GetMapping("/post")
    public String post(){
        return "board/post";
    }

    @PostMapping("/post")
    public String write(BoardDto boardDto) {
        System.out.println(boardDto);
        boardService.savePost(boardDto);
        return "redirect:/";
    }

    @GetMapping("/post/{id}")
    public String detail(@PathVariable("id") Long id, Model model){
        BoardDto boardDto = boardService.getPost(id);
        model.addAttribute("post",boardDto);
        return "board/detail";
    }

    @GetMapping("/post/edit/{id}")
    public String edit(@PathVariable("id") Long id, Model model){
        BoardDto boardDto = boardService.getPost(id);
        System.out.println(boardDto);
        System.out.println("Board Controller :: edit success");
        model.addAttribute("post",boardDto);
        return "board/edit";
    }

    @PutMapping("/post/edit/{id}")
    public String update(BoardDto boardDto) {
        boardService.savePost(boardDto);
        return "redirect:/";
    }

    @DeleteMapping("/post/{id}")
    public String delete(@PathVariable("id")Long id){
        boardService.deletePost(id);
        System.out.println("Board Controller :: delete success");
        return "redirect:/";
    }
    //https://caniro.tistory.com/6
    //https://tyrannocoding.tistory.com/13

    @GetMapping("/test")
    public String test(){
        return "board/summernote";
    }

    @GetMapping("/postT/{id}/category/{categoryId}")
    public String paramTest(@PathVariable("id")Long id, @PathVariable("categoryId") int categoryId){

        System.out.println("id: "+id+" categoryId: "+categoryId);
        return "redirect:/";
    }
}

