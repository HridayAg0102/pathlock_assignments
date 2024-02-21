from fastapi import FastAPI
from typing import List
from pydantic import BaseModel

app = FastAPI()

# Sample data for users
sample_users = [
    {"name": "Frank", "email": "frank@example.com", "phone": "444-555-6666", "age": 50},
    {"name": "Alice", "email": "alice@example.com", "phone": "123-456-7890", "age": 25},
    {"name": "Bob", "email": "bob@example.com", "phone": "987-654-3210", "age": 30},
    {"name": "Charlie", "email": "charlie@example.com", "phone": "555-555-5555", "age": 35},
    {"name": "David", "email": "david@example.com", "phone": "111-222-3333", "age": 40},
    {"name": "Eve", "email": "eve@example.com", "phone": "999-888-7777", "age": 0},
    {"name": "Grace", "email": "grace@example.com", "phone": "777-666-5555", "age": 55},
    {"name": "Henry", "email": "henry@example.com", "phone": "666-777-8888", "age": 60},
    {"name": "Ivy", "email": "ivy@example.com", "phone": "888-999-0000", "age": 65},
    {"name": "Jack", "email": "jack@example.com", "phone": "123-987-6543", "age": 70}

]

# Response model for user details
class UserDetail(BaseModel):
    name: str
    email: str
    phone: str
    age: int

# Route to get list of users and their details
@app.get("/users", response_model=List[UserDetail])
def get_users():
    return sample_users

if __name__ == "__main__":
    import uvicorn
    from pydantic import BaseModel
    uvicorn.run(app, host="0.0.0.0", port=8000)
