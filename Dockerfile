FROM fedora:latest

# create user-specific images
ARG UID
ARG GID
ARG USER
ARG GROUP

# 还是清华源快啊
RUN sed -e 's|^metalink=|#metalink=|g' \
    -e 's|^#baseurl=http://download.example/pub/fedora/linux|baseurl=https://mirrors.tuna.tsinghua.edu.cn/fedora|g' \
    -i.bak \
    /etc/yum.repos.d/fedora.repo \
    /etc/yum.repos.d/fedora-updates.repo

# Difftastic (a syntax-aware diff tool)
RUN dnf update -y && \
    dnf install -y clear awk wget g++ clang clang-tools-extra cmake vim git zip pip difft ninja

RUN pip install --upgrade pip && \
    pip install ruff uv

RUN echo "%wheel ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers.d/10-nopasswd

# 创建用户和组（确保容器内有 useradd 或 adduser 命令）
RUN groupadd -g ${GID} ${GROUP} && \
    useradd -u ${UID} -g ${GID} -m ${USER} && \
    usermod -aG wheel ${USER}

USER ${USER}


# 这个vcpkg也太傻逼了，他运行的时候会在仓库目录下创建文件。。。无语
# 那只能切换用户之后，在用户目录下面clone这个仓库了
# install vcpkg
# RUN mkdir -p /opt/vcpkg && \
#     git clone https://github.com/microsoft/vcpkg.git /opt/vcpkg && \
#     /opt/vcpkg/bootstrap-vcpkg.sh
RUN mkdir -p ~/.local/opt/vcpkg && \
    git clone https://github.com/microsoft/vcpkg.git ~/.local/opt/vcpkg && \
    ~/.local/opt/vcpkg/bootstrap-vcpkg.sh
# The environment variables set with export in a RUN command only exist within that specific RUN instruction and don't persist in the final container environment. This happens because each RUN instruction in a Dockerfile is executed in its own shell session, and the environment is not carried forward between them.
# To fix this and make VCPKG_ROOT available in your user's shell, use the ENV instruction in your Dockerfile instead:
# export VCPKG_ROOT=/opt/vcpkg && \
# export PATH=$PATH:$VCPKG_ROOT
# ENV VCPKG_ROOT=/opt/vcpkg
# ENV PATH=$PATH:$VCPKG_ROOT
# Set environment variables
ENV VCPKG_ROOT=/home/${USER}/.local/opt/vcpkg
ENV PATH=$PATH:$VCPKG_ROOT

WORKDIR /home/${USER}/algorithm

# An ENTRYPOINT allows you to configure a container that will run as an executable.
# 必须启动一个进程，否则容器启动就会立刻退出
# ENTRYPOINT ["./entrypoint.sh"]
ENTRYPOINT ["/bin/bash", "-c", "tail -f /dev/null"]
# ENTRYPOINT ["/bin/bash"]
