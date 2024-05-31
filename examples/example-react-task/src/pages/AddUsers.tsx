// src/pages/AddUsers.tsx
import React from 'react';
import AddUserForm from '../components/AddUsers';
import { useUserManager } from '../hooks/useUserManager'; // Assuming you have a hook for managing users
import styles from '../styles/TaskPage.module.css';

/**
 * Displays the list of users as well as buttons for performing
 * sync-related operations.
 */
export function AddUsersPage() {
    const { users, addUser, deleteUser } = useUserManager();

    const handleSubmit = (user: { name: string; userId: string }) => {
        // Add user to the list of users
        addUser(user.name, user.userId);
        console.log('Submitted user:', user);
    };

    return (
        <div className={styles.container}>
            <AddUserForm onSubmit={handleSubmit} />
            <ul>
                {users.map((user) => (
                    <li key={user._id.toString()}>
                        {user.name} ({user.userId})
                        <button onClick={() => deleteUser(user)}>Delete</button>
                    </li>
                ))}
            </ul>
        </div>
    );
}

export default AddUsersPage;
