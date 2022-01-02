package com.homepageTest.board.controller;

import com.homepageTest.board.domain.entity.Board;
import com.homepageTest.board.dto.BoardDto;
import com.homepageTest.board.service.BoardService;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;

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

    @PostMapping("/post/{id}")
    public String delete(@PathVariable("id")Long id){
        boardService.deletePost(id);
        System.out.println("Board Controller :: delete success");
        return "redirect:/";
    }
}

