# 分支战略 和 GitHub Flow

团队使用 Git 开发时，会使用不同的分支管理方法。  
这种开发时的分支运用方式称为：**ブランチ戦略**

**[ブランチ戦略](#ブランチ戦略)**  
**[GitHub Flow](#github-flow)**  
**[mainブランチ](#mainブランチについて)**  
**[GitHub Flowの流れ](#github-flowの流れ)**  
**[GitHub Flowの利点](#github-flowの利点)**  
**[Git-flow](#git-flow)**  

---

## 1. GitHub Flow

GitHub Flow 是最常用且最简单的 ブランチ戦略 之一。

其主要使用：

`main`  
`feature/xxx`

两种分支进行开发。

- ### feature

    `feature/`

    属于功能开发分支，后面通常接功能名。

    `feature/enemy1`  
    `feature/particle-editor`

    一个 feature 分支通常负责一个功能。

---

- ### main

    `main`

    默认主分支。

    通常：

    `main = 当前稳定版本`

    feature 分支开发完成后：

    `merge 回 main`

GitHub Flow 会保持：

`main 始终保持稳定、可运行`

---

## 2. GitHub Flow 流程

- 1. 从 main 分支创建 feature 分支，**创建前先 `pull` 最新 main**

---

- 2. 在 feature 分支开发功能并 commit，开发期间 **不要直接修改 main**

---

- 3. `push` feature 分支，并在 GitHub 上创建 **Pull Request**，请求负责人进行 Review 与 Merge

---

- 4. merge负责人需要解决冲突（conflict），并 **确认程序正常运行**，之后再 merge / push main

---

- 5. 功能完成后：删除远程和本地的 feature 分支，避免 branch 过多导致管理混乱

---

## 3. GitHub Flow 的利点

GitHub Flow 的特点：

- 结构简单
- 容易理解
- 适合小团队
- main 较稳定
- 适合持续开发

因此：

很多学校项目、小团队项目都会使用 GitHub Flow。

---

> 关于 Git-flow 和 GitHub Flow 的区别
>
> Git-flow 属于更复杂的分支战略。
>
> 除了：
>
> `main`  
> `feature`
>
> 还会使用：
>
> `develop`  
> `release`  
> `hotfix`
>
> 等更多分支。
>
> GitHub Flow 更简单，适合目前课程与小团队开发。  
> 目前暂时主要使用 GitHub Flow。