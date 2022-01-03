package com.homepageTest.board.domain.entity;

import lombok.AccessLevel;
import lombok.Builder;
import lombok.Getter;
import lombok.NoArgsConstructor;
import org.springframework.data.annotation.CreatedDate;
import org.springframework.data.annotation.LastModifiedDate;
import org.springframework.data.jpa.domain.support.AuditingEntityListener;

import javax.persistence.*;
import java.time.LocalDateTime;

@Getter //롬복 사용 getter setter 메소드 자동으로 관리해줌
@Entity
@NoArgsConstructor(access = AccessLevel.PROTECTED) //@builder 쓰려면 매개변수가 없는 기본 생성자를 무조건 선언해줘야함. 권한은 protected
@EntityListeners(AuditingEntityListener.class)
public class Board {

    @Id
    @GeneratedValue
    private Long id;


    @Column(length = 10, nullable=false)
    private String author;

    @Column(length = 100, nullable = false)
    private String title;

    @Column(columnDefinition = "TEXT", nullable = false)
    private String content;

    @CreatedDate
    @Column(updatable = false)
    private LocalDateTime createdDate;

    @LastModifiedDate
    private LocalDateTime modifiedDate;

    @Builder
    public Board(Long id, String author, String title, String content) {
        System.out.println("Board :: 생성자");
        this.id = id;
        this.author = author;
        this.title = title;
        this.content = content;
    }
}
