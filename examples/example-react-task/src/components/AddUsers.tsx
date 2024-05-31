// src/components/AddUserForm.tsx
import React, { FormEvent, useState } from 'react';
import styles from '../styles/AddUsers.module.css';

type AddUserFormProps = {
    onSubmit: (user: { name: string; userId: string }) => void;
};

/**
 * Form for adding a new user.
 */
const AddUserForm: React.FC<AddUserFormProps> = ({ onSubmit }) => {
    const [name, setName] = useState<string>('');
    const [userId, setUserId] = useState<string>('');

    const handleSubmit = (event: FormEvent<HTMLFormElement>): void => {
        event.preventDefault();
        onSubmit({ name, userId });
        setName('');
        setUserId('');
    };

    return (
        <form className={styles.form} onSubmit={handleSubmit}>
            <input
                className={styles.input}
                type='text'
                placeholder='Enter name'
                value={name}
                onChange={(event) => setName(event.currentTarget.value)}
            />
            <input
                className={styles.input}
                type='text'
                placeholder='Enter user ID'
                value={userId}
                onChange={(event) => setUserId(event.currentTarget.value)}
            />
            <button className={styles.button} type='submit'>
                ＋
            </button>
        </form>
    );
};

export default AddUserForm;
