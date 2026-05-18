# GitHub Flow

**[ブランチ戦略](#ブランチ戦略)**  
**[GitHub Flow](#github-flow)**  
**[mainブランチ](#mainブランチについて)**  
**[GitHub Flowの流れ](#github-flowの流れ)**  
**[GitHub Flowの利点](#github-flowの利点)**  
**[Git-flow](#git-flow)**  

---

## ブランチ戦略

团队使用 Git 开发时，会使用不同的分支管理方法。

这种开发时的分支运用方式称为：

>ブランチ戦略（branch strategy）

---

## GitHub Flow

GitHub Flow 是最常用且最简单的分支战术之一。

主要使用：

- main
- feature/xxx

这两种分支进行开发。

### feature 分支命名

feature 后面通常会接功能名。

例如：

```text
feature/enemy1
feature/particle-editor
```

这样看到分支名就能立刻知道：

>“这个分支是在开发什么功能”

---

## mainブランチについて

GitHub 从 2020 年开始：

>默认分支名称从 master 改为 main

原因是：

>master/slave 容易联想到奴隶制度

不过：

- 现在很多旧项目仍然使用 master
- 功能上没有区别

因此：

```text
master = main
```

可以简单理解为：

>默认分支

---

## GitHub Flowの流れ

### ① 从 main 分支创建 feature 分支

创建分支前：

>先 pull 最新 main

---

### ② 在 feature 分支开发功能并 commit

功能开发期间：

>不要直接修改 main

---

### ③ push feature 分支并创建 Pull Request

将功能提交给：

- 团队成员
- Merge负责人

进行确认。

---

### ④ Merge 到 main

Merge 时通常需要：

- 解决冲突（conflict）
- 确认程序正常运行

之后再 push main。

---

### ⑤ 删除远程 feature 分支

功能完成后：

>删除已经 merge 的远程分支

---

### ⑥ 删除本地 feature 分支

保持分支结构整洁。

---

## GitHub Flowの利点

### main 分支始终保持可运行

游戏开发中：

>开发中的功能可能暂时无法运行

但 GitHub Flow 会保持：

```text
main 始终是稳定状态
```

---

### 分支结构简单

功能完成后会删除 feature 分支。

因此：

>仓库不会堆积大量旧分支

---

### 学习成本低

GitHub Flow 规则简单：

- 好理解
- 好维护
- 适合小团队与学生开发

---

## Git-flow

Git-flow 是另一种著名分支战略。

相比 GitHub Flow：

>结构更复杂

通常会使用：

- main
- develop
- release
- feature
- hotfix

等多个分支。

---

### Git-flow特点

适合：

- 大型团队
- 长期持续开发
- 多版本维护

但对于学生开发：

>通常有些过度复杂（オーバースペック）

---

## キーワード（关键词）

| 中文 | 日语 |
|---|---|
| 分支战略 | ブランチ戦略 |
| 默认分支 | main / master |
| 功能分支 | featureブランチ |
| 拉取请求 | Pull Request |
| 合并 | Merge |
| 冲突 | conflict |
| 推送 | push |
| 提交 | commit |
| 拉取 | pull |
| GitHub工作流 | GitHub Flow |
| Git-flow | Git-flow |
| 热修复 | hotfix |
| 发布分支 | release |
| 开发分支 | develop |