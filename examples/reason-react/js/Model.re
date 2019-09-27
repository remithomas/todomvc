type todoId = string;

type todo = {
	id: todoId,
	title: string,
  	completed: bool
}

type filter =
	| All
	| Active
	| Completed;
