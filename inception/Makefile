NAME = inception

all: $(NAME)

$(NAME):
	@mkdir -p /home/jamorale/data/db
	@mkdir -p /home/jamorale/data/website-files
	@echo "Bulding containers..."
	@docker compose -f srcs/docker-compose.yml up --build

clean:
	@echo "Shutting down containers..."
	@docker compose -f srcs/docker-compose.yml down

fclean: clean
	@echo "Pruning images..."
	@docker image prune -af > /dev/null 2>&1
	@echo "Removing db files..."
	@docker volume rm -f mariadb_data > /dev/null 2>&1
	@rm -rf /home/jamorale/data/db
	@echo "Removing Wordpress files..."
	@docker volume rm -f wordpress_data > /dev/null 2>&1
	@rm -rf /home/jamorale/data/website-files

re: fclean all

exec_wordpress:
	@docker exec -it wordpress sh

exec_nginx:
	@docker exec -it nginx sh

exec_mariadb:
	@docker exec -it mariadb sh

.PHONY: inception all clean fclean re exec_wordpress exec_nginx exec_mariadb
