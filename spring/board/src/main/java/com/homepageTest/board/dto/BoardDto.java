package com.homepageTest.board.dto;

import com.homepageTest.board.domain.entity.Board;
import lombok.*;

import java.time.LocalDateTime;


@Getter
@Setter
@ToString
@NoArgsConstructor
public class BoardDto {

    private Long id;
    private String author;
    private String title;
    private String content;
    private LocalDateTime createdDate;
    private LocalDateTime modifiedDate;

    public Board toEntity() { //Board에서 매개변수로 받아온 데이터 매핑해줌
        Board build = Board.builder()
                .id(id)
                .author(author)
                .title(title)
                .content(content)
                .build();
        return build;
    }

    @Builder
    public BoardDto(Long id, String author, String title, String content, LocalDateTime createdDate, LocalDateTime modifiedDate) {
        System.out.println("BoardDto :: 생성자");
        this.id = id;
        this.author = author;
        this.title = title;
        this.content = content;
        this.createdDate = createdDate;
        this.modifiedDate = modifiedDate;
    }
}
