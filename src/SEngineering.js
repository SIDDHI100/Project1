import React from "react";
import { Navbar, Nav, NavDropdown, Form, Button, Container, Card } from "react-bootstrap";
import './project1.css'; 

const service = {
    name: "Amazing Web Development Service",
    description: "We provide cutting-edge web development services for businesses of all sizes.",
    details: [
        "Custom website design",
        "E-commerce solutions",
        "SEO optimization",
        "Mobile-responsive designs",
    ],
    founder: {
        name: "Atharv Sankpal",
        background: "Atharv has 10+ years of experience in web development, specializing in JavaScript and modern frameworks.",
        image: "https://via.placeholder.com/150"  // Placeholder image URL
    },
    contact: {
        email: "contact@amev.com",
        phone: "+91-9665593641",
        website: "https://www.amazingwebdev.com",
    },
};

const NavScrollExample = () => {
    return (
        <Navbar expand="lg" className="bg-body-tertiary">
            <Container fluid>
                <Navbar.Brand href="#">Navbar scroll</Navbar.Brand>
                <Navbar.Toggle aria-controls="navbarScroll" />
                <Navbar.Collapse id="navbarScroll">
                    <Nav
                        className="me-auto my-2 my-lg-0"
                        style={{ maxHeight: '100px' }}
                        navbarScroll
                    >
                        <Nav.Link href="#action1">Home</Nav.Link>
                        <Nav.Link href="#action2">Link</Nav.Link>
                        <NavDropdown title="Link" id="navbarScrollingDropdown">
                            <NavDropdown.Item href="#action3">Action</NavDropdown.Item>
                            <NavDropdown.Item href="#action4">Another action</NavDropdown.Item>
                            <NavDropdown.Divider />
                            <NavDropdown.Item href="#action5">Something else here</NavDropdown.Item>
                        </NavDropdown>
                        <Nav.Link href="#" disabled>
                            Link
                        </Nav.Link>
                    </Nav>
                    <Form className="d-flex">
                        <Form.Control
                            type="search"
                            placeholder="Search"
                            className="me-2"
                            aria-label="Search"
                        />
                        <Button variant="outline-success">Search</Button>
                    </Form>
                </Navbar.Collapse>
            </Container>
        </Navbar>
    );
};

const SEngineering = () => {
    return (
        <>
            <NavScrollExample />
            <div className="service-details">
                <h1>{service.name}</h1>
                <p className="description">{service.description}</p>

                <div className="service-info">
                    <h2>Service Details</h2>
                    <ul>
                        {service.details.map((detail, index) => (
                            <li key={index}>{detail}</li>
                        ))}
                    </ul>
                </div>

                <div className="founder-info">
                    <h2>Founder</h2>

                    {/* Interactive Image Card */}
                    <Card className="founder-card">
                        <Card.Img 
                            variant="top" 
                            src={service.founder.image} 
                            alt="Founder Image" 
                            className="founder-image" 
                        />
                        <Card.Body>
                            <Card.Title>{service.founder.name}</Card.Title>
                            <Card.Text>{service.founder.background}</Card.Text>
                        </Card.Body>
                    </Card>
                </div>

                <div className="contact-info">
                    <h2>Contact Details</h2>
                    <p><strong>Email:</strong> {service.contact.email}</p>
                    <p><strong>Phone:</strong> {service.contact.phone}</p>
                    <p><strong>Website:</strong> <a href={service.contact.website}>{service.contact.website}</a></p>
                </div>
            </div>
        </>
    );
};

export default SEngineering;
